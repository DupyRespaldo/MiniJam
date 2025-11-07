// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "minijamPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AminijamPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// tecla uno para viajar al mapa
	UFUNCTION()
	void HandleStartKey();
	
	UFUNCTION(Server, Reliable)
	void Server_StartMatch(const FString& MapURL);

protected:
	// no funciona :/
	UPROPERTY(EditDefaultsOnly, Category="Lobby")
	FString LavaPitMapURL = TEXT("/Game/Levels/L1_LavaPit?listen");

	// ServerTravel
	void TravelToMap(const FString& MapURL);

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
};
