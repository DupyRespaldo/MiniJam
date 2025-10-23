#pragma once

#include "CoreMinimal.h"
#include "CollisionHandler.h"
#include "Pilar.generated.h"

UCLASS()
class TUJUEGO_API APilar : public ACollisionHandler
{
	GENERATED_BODY()

public:
	APilar();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, Replicated, Category = "Pilar")
	float OscillationHeight = 100.f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Pilar")
	float Speed = 2.0f;

	bool bPlayerOnTop = false;
	FVector InitialLocation;

public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
