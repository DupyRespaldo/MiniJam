// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boton.generated.h"

UCLASS()
class MINIJAM_API ABoton : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoton();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Portal")
	class UStaticMeshComponent* Mesh;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
	class UUserWidget* ActiveWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FName LevelToLoad;
	
	UPROPERTY()
	class ACharacter* OverlappingPlayer;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(Server, Reliable)
	void Server_LoadLevel();

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
};