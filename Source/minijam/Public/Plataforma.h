// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionHandler.h"
#include "Plataforma.generated.h"

UCLASS()
class MINIJAM_API APlataforma : public ACollisionHandler
{
	GENERATED_BODY()
	
public:
	APlataforma();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Platform")
	float DisappearTime = 3.0f;
	
	UPROPERTY(EditAnywhere, Category = "Platform")
	UMaterialInterface* MaterialNormal;

	UPROPERTY(EditAnywhere, Category = "Platform")
	UMaterialInterface* MaterialBlink;

	FTimerHandle TimerDisappear;
	FTimerHandle TimerReset;
	FTimerHandle TimerBlink;

	bool bIsBlinking = false;

	void StartBlinking();
	void ToggleMaterial();
	void Disappear();
	void ResetPlatform();
};