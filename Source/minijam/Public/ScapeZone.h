// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ScapeZone.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class MINIJAM_API AScapeZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScapeZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category="Component")
	TObjectPtr<UBoxComponent> OverlapComp;

	UPROPERTY(VisibleAnywhere, Category="Component")
	UDecalComponent* Decalcomp;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
