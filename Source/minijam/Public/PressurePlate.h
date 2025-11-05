// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

UCLASS()
class MINIJAM_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	APressurePlate();

	/** Registrar replicación */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Mesh y colisión (expuestos para editar en BP) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plate")
	class UStaticMeshComponent* PlateMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Plate")
	class UBoxComponent* TriggerBox;

	/** Actor que se destruirá cuando todas las placas estén presionadas. Se puede asignar en Details (por instancia) */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Category="Plate")
	AActor* TargetActor;

	/** Estado replicado: si la placa está actualmente presionada */
	UPROPERTY(ReplicatedUsing=OnRep_IsPressed, VisibleAnywhere, Category="Plate")
	bool bIsPressed;

	/** Número de pawns/characters actualmente sobre esta placa (solo servidor lo mantiene) */
	int32 LocalOverlappingCount;

	/** Lista global de placas (solo usada en servidor) */
	static TArray<APressurePlate*> AllPlates;

	/** ---- overlap callbacks ---- */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ServerSetPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerSetPressed_Implementation(bool bPressed);
	
	UFUNCTION()
	void OnRep_IsPressed();
	static void CheckAllPlatesPressed_Server();
	void HandleAllPlatesPressed();

public:	
	virtual void Tick(float DeltaTime) override;
};
