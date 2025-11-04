#include "Pilar.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

APilar::APilar()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PilarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PilarMesh"));
	RootComponent = PilarMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APilar::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APilar::OnOverlapEnd);

	MoveSpeed = 1.f;
	LowerDistance = 200.f;
	bIsLowering = false;
}

void APilar::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	CurrentLocation = InitialLocation;
}

void APilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) 
	{
		HandleMovement();
	}
	else 
	{
		SetActorLocation(CurrentLocation);
	}
}

void APilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor != this)
	{
		bIsLowering = true;
	}
}

void APilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() && OtherActor && OtherActor != this)
	{
		bIsLowering = false;
	}
}

void APilar::HandleMovement()
{
	FVector TargetLocation = InitialLocation;

	if (bIsLowering)
		TargetLocation.Z -= LowerDistance;

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), MoveSpeed);
	SetActorLocation(NewLocation);

	CurrentLocation = NewLocation; 
}

void APilar::OnRep_PilarLocation()
{
	SetActorLocation(CurrentLocation);
}

void APilar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APilar, InitialLocation);
	DOREPLIFETIME(APilar, CurrentLocation);
	DOREPLIFETIME(APilar, bIsLowering);
}
