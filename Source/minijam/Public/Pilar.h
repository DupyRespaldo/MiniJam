// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pilar.generated.h"

UCLASS()
class MINIJAM_API APilar : public AActor
{
	GENERATED_BODY()

public:
	APilar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Pilar")
	class UBoxComponent* TriggerBox;
	
	UPROPERTY(VisibleAnywhere, Category = "Pilar")
	class UStaticMeshComponent* PilarMesh;
	
	UPROPERTY(Replicated)
	FVector InitialLocation;
	
	UPROPERTY(ReplicatedUsing = OnRep_PilarLocation)
	FVector CurrentLocation;
	
	UPROPERTY(EditAnywhere, Category = "Pilar")
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Pilar")
	float LowerDistance;
	
	UPROPERTY(Replicated)
	bool bIsLowering;

	FTimerHandle MovementTimer;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void HandleMovement();

	UFUNCTION()
	void OnRep_PilarLocation();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
