// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zone.generated.h"

UCLASS()
class MINIJAM_API AZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AZone();

protected:
	virtual void BeginPlay() override;

	// Colisión
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float DamageInterval;

	FTimerHandle DamageTimerHandle;

	UPROPERTY()
	AActor* OverlappingActor;

	void ApplyDamage();

	// Eventos de overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
