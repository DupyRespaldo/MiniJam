// Fill out your copyright notice in the Description page of Project Settings.


#include "ScapeZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "minijam/minijamCharacter.h"
#include "minijam/minijamGameMode.h"

// Sets default values
AScapeZone::AScapeZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component")); OverlapComp->SetCollisionEnabled( ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToChannels ( ECR_Overlap); OverlapComp->SetCollisionResponseToChannel (ECC_Pawn, ECR_Overlap);
	RootComponent = OverlapComp;
	OverlapComp->SetHiddenInGame(false);
	
	Decalcomp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decalcomp"));
	Decalcomp-> DecalSize = FVector(200);
	Decalcomp-> SetupAttachment(RootComponent);
}

void AScapeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScapeZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap);
}

void AScapeZone::HandleOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor:: Green,FString::Printf(  TEXT("Overlapped")));
	}
	AminijamCharacter* MyPawn = Cast<AminijamCharacter>(OtherActor);
	if (MyPawn == nullptr) return;
	if (MyPawn->bCarryObjetive)
	{
		AminijamGameMode* GM = Cast<AminijamGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
		
	}
}

