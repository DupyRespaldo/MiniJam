#include "PressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

TArray<APressurePlate*> APressurePlate::AllPlates;

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false; // no hace falta tick
	bReplicates = true;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	RootComponent = PlateMesh;
	PlateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlateMesh->SetCollisionResponseToAllChannels(ECR_Block);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(50.f,50.f,20.f));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnEndOverlap);

	bIsPressed = false;
	LocalOverlappingCount = 0;
	TargetActor = nullptr;
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AllPlates.Add(this);
	}
	OnRep_IsPressed();
}

void APressurePlate::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Eliminar de lista global si estamos en servidor
	if (HasAuthority())
	{
		AllPlates.Remove(this);
	}
	Super::EndPlay(EndPlayReason);
}

void APressurePlate::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	// Solo NPCs/jugadores que sean Characters cuentan
	if (!HasAuthority()) return;

	if (OtherActor && OtherActor->IsA<ACharacter>())
	{
		LocalOverlappingCount++;
		if (LocalOverlappingCount == 1)
		{
			ServerSetPressed(true);
		}
	}
}

void APressurePlate::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (OtherActor && OtherActor->IsA<ACharacter>())
	{
		LocalOverlappingCount = FMath::Max(0, LocalOverlappingCount - 1);
		if (LocalOverlappingCount == 0)
		{
			ServerSetPressed(false);
		}
	}
}

void APressurePlate::ServerSetPressed_Implementation_Implementation(bool bPressed)
{
	// Solo aplicar si cambia
	if (bIsPressed == bPressed) return;

	bIsPressed = bPressed;

	// Llamar OnRep aquí también para reflectar en servidor
	OnRep_IsPressed();

	// Si se presionó, comprobar si todas las placas están presionadas
	if (bIsPressed)
	{
		CheckAllPlatesPressed_Server();
	}
}

void APressurePlate::ServerSetPressed(bool bPressed)
{
	// Si estamos en servidor, la implementación se ejecutará en ServerSetPressed_Implementation
	if (HasAuthority())
	{
		ServerSetPressed_Implementation(bPressed);
	}
	else
	{
		ServerSetPressed_Implementation(bPressed);
	}
}

void APressurePlate::OnRep_IsPressed()
{
	if (PlateMesh)
	{
		PlateMesh->SetScalarParameterValueOnMaterials(TEXT("Pressed"), bIsPressed ? 1.0f : 0.0f);
	}
}

void APressurePlate::CheckAllPlatesPressed_Server()
{
	// Solo el servidor ejecuta esto
	if (!GEngine) {} 
	UWorld* World = nullptr;
	if (AllPlates.Num() == 0) return;


	for (APressurePlate* Plate : AllPlates)
	{

		if (!Plate) continue;


		if (!Plate->bIsPressed)
		{
			return;
		}
	}
	
	for (APressurePlate* Plate : AllPlates)
	{
		if (Plate && Plate->TargetActor)
		{
			// Destruir el target en servidor
			if (Plate->TargetActor->HasAuthority())
			{
				Plate->TargetActor->Destroy();
			}
			else
			{
				Plate->TargetActor->Destroy();
			}
			break;
		}
	}
}

void APressurePlate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APressurePlate, bIsPressed);
	DOREPLIFETIME(APressurePlate, TargetActor);
}
