// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plataforma.generated.h"

UCLASS()
class MINIJAM_API APlataforma : public AActor
{
	GENERATED_BODY()
	
public:
	APlataforma();

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Platform")
	float DisappearTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Platform")
	UMaterialInterface* MaterialNormal;

	UPROPERTY(EditAnywhere, Category = "Platform")
	UMaterialInterface* MaterialBlink;

	UPROPERTY(ReplicatedUsing = OnRep_IsHidden)
	bool bIsHidden = false;

	UPROPERTY(Replicated)
	int32 OverlappingPlayers = 0; // cantidad de jugadores dentro

	FTimerHandle TimerDisappear;
	FTimerHandle TimerReset;
	FTimerHandle TimerBlink;

	bool bIsBlinking = false;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_IsHidden();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartBlinking();

	void ToggleMaterial();
	void Disappear();
	void ResetPlatform();
};
