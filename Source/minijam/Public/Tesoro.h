
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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};