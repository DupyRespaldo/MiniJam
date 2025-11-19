
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tesoro.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()

class MINIJAM_API ATesoro : public AActor
{
	GENERATED_BODY()
public:
	ATesoro();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* PickUpVFX;
	void PlayEffects();
	UPROPERTY(EditDefaultsOnly, Category="Treasure")
	FName TreasureTag = "HasTreasure";

	UPROPERTY(EditDefaultsOnly, Category="Treasure")
	bool bDestroyOnPickup = true;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& SweepResult);
	
};