#include "CollisionHandler.h"
#include "Components/BoxComponent.h"

ACollisionHandler::ACollisionHandler()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetIsReplicated(true);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollisionHandler::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACollisionHandler::OnEndOverlap);
}

void ACollisionHandler::BeginPlay()
{
	Super::BeginPlay();
}

void ACollisionHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollisionHandler::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									   const FHitResult& SweepResult)
{
}

void ACollisionHandler::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
