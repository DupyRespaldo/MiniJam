// Copyright Epic Games, Inc. All Rights Reserved.

#include "minijamCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "minijam.h"

AminijamCharacter::AminijamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AminijamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AminijamCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AminijamCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AminijamCharacter::Look);

		//ThrowAction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AminijamCharacter::TryCarry);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &AminijamCharacter::DropOrThrowTrue);

	}
	else
	{
		UE_LOG(Logminijam, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AminijamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AminijamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AminijamCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AminijamCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AminijamCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AminijamCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AminijamCharacter::TryCarry()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("TryCarry pressed"));
    ServerTryCarry();
}

void AminijamCharacter::DropOrThrow(bool bThrow)
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("DropOrThrow called, bThrow = %s"), bThrow ? TEXT("true") : TEXT("false")));
    ServerDropOrThrow(bThrow);
}

void AminijamCharacter::DropOrThrowTrue()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("DropOrThrowTrue called"));
    DropOrThrow(true);
}

void AminijamCharacter::ServerTryCarry_Implementation()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("ServerTryCarry_Implementation called"));

    if (CarriedPlayer)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Already carrying someone"));
        return;
    }

    FVector Start = FollowCamera->GetComponentLocation();
    FVector End = Start + (FollowCamera->GetForwardVector() * CarryDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
    {
        AminijamCharacter* HitPlayer = Cast<AminijamCharacter>(Hit.GetActor());
        if (HitPlayer && !HitPlayer->bIsBeingCarried)
        {
            CarriedPlayer = HitPlayer;
            HitPlayer->bIsBeingCarried = true;
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player found and attached"));
            MulticastAttachPlayer(HitPlayer);
        }
        else
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Player found but already carried or invalid"));
        }
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No player hit in trace"));
    }
}

void AminijamCharacter::ServerDropOrThrow_Implementation(bool bThrow)
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("ServerDropOrThrow_Implementation called, bThrow = %s"), bThrow ? TEXT("true") : TEXT("false")));

    if (!CarriedPlayer)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No carried player to drop"));
        return;
    }

    AminijamCharacter* Target = CarriedPlayer;
    CarriedPlayer = nullptr;
    Target->bIsBeingCarried = false;

    MulticastDetachPlayer(bThrow);
}

void AminijamCharacter::MulticastAttachPlayer_Implementation(AminijamCharacter* Target)
{
    if (!Target)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("MulticastAttachPlayer: Target is null"));
        return;
    }

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("MulticastAttachPlayer executed"));

    FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
    Target->AttachToComponent(GetMesh(), AttachRules, FName("spine_03"));

    FVector Offset(0, 0, 100);
    Target->SetActorRelativeLocation(Offset);
    Target->GetCharacterMovement()->DisableMovement();
}

void AminijamCharacter::MulticastDetachPlayer_Implementation(bool bThrow)
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("MulticastDetachPlayer called, bThrow = %s"), bThrow ? TEXT("true") : TEXT("false")));

    if (!CarriedPlayer && !bIsBeingCarried)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No player to detach"));
        return;
    }

    ACharacter* Carrier = nullptr;
    if (bIsBeingCarried)
    {
        Carrier = Cast<AminijamCharacter>(GetAttachParentActor());
    }

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    if (bThrow && Carrier)
    {
        FVector ForwardDir = Carrier->GetActorForwardVector();
        LaunchCharacter(ThrowForce, true, true);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Player thrown"));
    }
}

void AminijamCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AminijamCharacter, CarriedPlayer);
    DOREPLIFETIME(AminijamCharacter, bIsBeingCarried);
}
