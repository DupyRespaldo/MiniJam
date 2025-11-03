// Fill out your copyright notice in the Description page of Project Settings.


#include "Tesoro.h"
#include "Components/SphereComponent.h"
#include "minijam/minijamCharacter.h"
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
}

void ATesoro::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATesoro::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,PickUpVFX,GetActorLocation());
}

void ATesoro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATesoro::NotifyActorBeginOverlap(class AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffects();

	AminijamCharacter* MyCharacter = Cast<AminijamCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->bCarryObjetive = true;
		Destroy();
	}
}
