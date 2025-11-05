#include "Plataforma.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

APlataforma::APlataforma()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlataforma::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APlataforma::OnEndOverlap);
}

void APlataforma::BeginPlay()
{
	Super::BeginPlay();
}

void APlataforma::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor->IsA<ACharacter>()) return;

	OverlappingPlayers++;

	// Si es el primer jugador, comienza la secuencia
	if (OverlappingPlayers == 1 && !bIsHidden)
	{
		GetWorldTimerManager().SetTimer(TimerBlink, [this]()
		{
			Multicast_StartBlinking();
		}, DisappearTime - 1.5f, false);

		GetWorldTimerManager().SetTimer(TimerDisappear, this, &APlataforma::Disappear, DisappearTime, false);
	}
}

void APlataforma::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority() || !OtherActor->IsA<ACharacter>()) return;

	OverlappingPlayers = FMath::Max(0, OverlappingPlayers - 1);
	
	if (OverlappingPlayers == 0 && bIsHidden)
	{
		GetWorldTimerManager().SetTimer(TimerReset, this, &APlataforma::ResetPlatform, 3.0f, false);
	}
	else if (OverlappingPlayers == 0)
	{
		GetWorldTimerManager().ClearTimer(TimerBlink);
		GetWorldTimerManager().ClearTimer(TimerDisappear);
		bIsBlinking = false;
		if (MaterialNormal) Mesh->SetMaterial(0, MaterialNormal);
	}
}

void APlataforma::Multicast_StartBlinking_Implementation()
{
	bIsBlinking = true;
	GetWorldTimerManager().SetTimer(TimerBlink, this, &APlataforma::ToggleMaterial, 0.2f, true);
}

void APlataforma::ToggleMaterial()
{
	if (!Mesh || !MaterialNormal || !MaterialBlink) return;

	static bool bAlt = false;
	Mesh->SetMaterial(0, bAlt ? MaterialNormal : MaterialBlink);
	bAlt = !bAlt;
}

void APlataforma::Disappear()
{
	if (HasAuthority())
	{
		bIsHidden = true;
		OnRep_IsHidden();

		GetWorldTimerManager().ClearTimer(TimerBlink);
	}
}

void APlataforma::OnRep_IsHidden()
{
	SetActorHiddenInGame(bIsHidden);
	SetActorEnableCollision(!bIsHidden);

	if (Mesh)
	{
		Mesh->SetVisibility(!bIsHidden);
		if (!bIsHidden && MaterialNormal)
		{
			Mesh->SetMaterial(0, MaterialNormal);
		}
	}
}

void APlataforma::ResetPlatform()
{
	if (HasAuthority())
	{
		bIsHidden = false;
		OnRep_IsHidden();
	}
}

void APlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlataforma::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlataforma, DisappearTime);
	DOREPLIFETIME(APlataforma, bIsHidden);
	DOREPLIFETIME(APlataforma, OverlappingPlayers);
}
