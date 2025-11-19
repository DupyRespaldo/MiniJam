// Fill out your copyright notice in the Description page of Project Settings.


#include "Boton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ABoton::ABoton()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(Mesh);
	TriggerBox->SetBoxExtent(FVector(150.f, 150.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABoton::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABoton::OnOverlapEnd);

	LevelToLoad = "YourLevelName";
}

void ABoton::BeginPlay()
{
	Super::BeginPlay();
}

void ABoton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Si hay un jugador dentro y presiona E hacer el viaje
}

void ABoton::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
}

void ABoton::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
}

void ABoton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		OverlappingPlayer = Player;
		// Logica de viaje nivel
	}
}

void ABoton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player && Player == OverlappingPlayer)
	{
		OverlappingPlayer = nullptr;

		if (ActiveWidget)
		{
			ActiveWidget->RemoveFromParent();
			ActiveWidget = nullptr;
		}
	}
}

void ABoton::Server_LoadLevel_Implementation()
{
	if (HasAuthority())
	{
		UGameplayStatics::OpenLevel(this, LevelToLoad, true);
	}
}
