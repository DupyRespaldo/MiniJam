// Fill out your copyright notice in the Description page of Project Settings.


#include "ScapeZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "minijam/minijamCharacter.h"
#include "minijam/minijamGameMode.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "minijamGameMode.h"
#include "minijam/minijamPlayerController.h"



AScapeZone::AScapeZone()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	BoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComp->SetGenerateOverlapEvents(true);

	bReplicates = true;
}

void AScapeZone::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AScapeZone::HandleOverlap);
}

void AScapeZone::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor:: Green,FString::Printf(  TEXT("Overlapped")));
		return;
	}

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	if (bRequireTreasure && !OtherActor->Tags.Contains(TreasureTag))
	{
		if (AminijamPlayerController* PC = Cast<AminijamPlayerController>(Pawn->GetController()))
		{
			PC->ClientShowCenterToast(FText::FromString(TEXT("YOU NEED A TREASURE TO ABLE ESCAPE")));
		}
		return;
	}
	
	if (AminijamGameMode* GM = GetWorld()->GetAuthGameMode<AminijamGameMode>())
	{
		GM->CompleteMission(Pawn); 
	}
}



