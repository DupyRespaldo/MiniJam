// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class MINIJAM_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();


protected:
	virtual void BeginPlay() override;

	/** Componente de colisión para detectar el overlap con el personaje */
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereComp;

	/** Malla visible del pickup */
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	UStaticMeshComponent* MeshComp;

	/** Se llama cuando otro actor entra en el SphereComp */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);
};
