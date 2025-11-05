// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Controller.h"

AZone::AZone()
{
		PrimaryActorTick.bCanEverTick = false;
		bReplicates = true;

		DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
		RootComponent = DamageBox;

		DamageBox->InitBoxExtent(FVector(50.f, 60.f, 10.f)); 
		DamageBox->SetCollisionProfileName(TEXT("Trigger"));

		//overlap
		DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AZone::OnOverlapBegin);
		DamageBox->OnComponentEndOverlap.AddDynamic(this, &AZone::OnOverlapEnd);


		DamageType = UDamageType::StaticClass();
		DamageAmount = 25.f;
		DamageInterval = 2.f;
		OverlappingActor = nullptr;
	}

	void AZone::BeginPlay()
	{
		Super::BeginPlay();
	}

	void AZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									 bool bFromSweep, const FHitResult& SweepResult)
	{
		if (OtherActor && OtherActor != this)
		{
			OverlappingActor = OtherActor;
			GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AZone::ApplyDamage, DamageInterval, true, 0.0f);
		}
	}

	void AZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
	{
		if (OtherActor && OtherActor == OverlappingActor)
		{
			GetWorldTimerManager().ClearTimer(DamageTimerHandle);
			OverlappingActor = nullptr;
		}
	}

	void AZone::ApplyDamage()
	{
		if (OverlappingActor)
		{
			UGameplayStatics::ApplyDamage(OverlappingActor, DamageAmount, GetInstigatorController(), this, DamageType);
		}
	}
