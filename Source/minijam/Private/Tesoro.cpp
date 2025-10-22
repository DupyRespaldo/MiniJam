// Fill out your copyright notice in the Description page of Project Settings.


#include "Tesoro.h"

#include "Components/SphereComponent.h"
#include "minijam/minijamCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATesoro::ATesoro()
{
	bReplicates = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Solo es un registro de los actores
	SphereComp->SetCollisionResponseToChannels(ECR_Ignore); //Toma todos los canales de colición y los ignora
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap); //Solo el Pawn puede hacer overlab
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATesoro::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATesoro::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,PickUpVFX,GetActorLocation());
}


// Called every frame
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
