#include "Plataforma.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

APlataforma::APlataforma()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	// Mesh principal
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Collider
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
	if (HasAuthority())
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
	// No hace nada al salir — solo reaparece después del timer
}

void APlataforma::Multicast_StartBlinking_Implementation()
{
	bIsBlinking = true;
	GetWorldTimerManager().SetTimer(TimerBlink, this, &APlataforma::ToggleMaterial, 0.2f, true);
}

void APlataforma::ToggleMaterial()
{
	if (!Mesh || !MaterialNormal || !MaterialBlink) return;

	static bool bUseAlt = false;
	Mesh->SetMaterial(0, bUseAlt ? MaterialNormal : MaterialBlink);
	bUseAlt = !bUseAlt;
}

void APlataforma::Disappear()
{
	if (HasAuthority())
	{
		bIsHidden = true;
		OnRep_IsHidden();

		GetWorldTimerManager().ClearTimer(TimerBlink);
		GetWorldTimerManager().SetTimer(TimerReset, this, &APlataforma::ResetPlatform, 5.0f, false);
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
}
