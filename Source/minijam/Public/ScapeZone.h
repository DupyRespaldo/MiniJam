// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScapeZone.generated.h"

class UBoxComponent;

UCLASS()
class MINIJAM_API AScapeZone : public AActor
{
	GENERATED_BODY()

public:
	AScapeZone();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Victory")
	bool bRequireTreasure = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Victory")
	FName TreasureTag = "HasTreasure";

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& SweepResult);
};
