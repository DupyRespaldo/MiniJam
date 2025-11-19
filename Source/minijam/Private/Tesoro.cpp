// Fill out your copyright notice in the Description page of Project Settings.


#include "Tesoro.h"
#include "Components/SphereComponent.h"
#include "minijam/minijamCharacter.h"
#include "minijam/minijamPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

ATesoro::ATesoro()
{
	bReplicates = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToChannels(ECR_Ignore); 
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap); 
	SphereComp->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = false;

	// PLACEHOLDER
	USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Sphere->InitSphereRadius(60.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Sphere->SetGenerateOverlapEvents(true);
	SetRootComponent(Sphere);
}

void ATesoro::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,PickUpVFX,GetActorLocation());
}

void ATesoro::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* Collision = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ATesoro::HandleOverlap);
	}
}

void ATesoro::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	if (!OtherActor->Tags.Contains(TreasureTag))
	{
		OtherActor->Tags.Add(TreasureTag);
	}

	if (AminijamPlayerController* PC = Cast<AminijamPlayerController>(Pawn->GetController()))
	{
		PC->ClientShowCenterToast(FText::FromString(TEXT("GOT THE TREASUERE")));
	}

	if (bDestroyOnPickup)
	{
		Destroy();
	}
}