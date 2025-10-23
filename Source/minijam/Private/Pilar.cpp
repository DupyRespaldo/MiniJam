#include "Pilar.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

APilar::APilar()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
}

void APilar::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
}

void APilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	FVector NewLocation = GetActorLocation();

	if (bPlayerOnTop)
	{
		NewLocation.Z -= Speed * 20 * DeltaTime;
	}
	else
	{
		float DeltaZ = FMath::Sin(GetWorld()->GetTimeSeconds() * Speed) * OscillationHeight;
		NewLocation.Z = InitialLocation.Z + DeltaZ;
	}

	SetActorLocation(NewLocation);
}

void APilar::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	bPlayerOnTop = true;
}

void APilar::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bPlayerOnTop = false;
}

void APilar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APilar, OscillationHeight);
	DOREPLIFETIME(APilar, Speed);
}
