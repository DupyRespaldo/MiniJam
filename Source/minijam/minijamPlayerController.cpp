// Copyright Epic Games, Inc. All Rights Reserved.

#include "minijamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "minijam.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "InputCoreTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void AminijamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);
		}
		else
		{
			UE_LOG(Logminijam, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
}

void AminijamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}

		// tecla uno para viajar
		InputComponent->BindKey(EKeys::One, IE_Pressed, this, &AminijamPlayerController::HandleStartKey);
	}
}

// por seguridad
void AminijamPlayerController::HandleStartKey()
{
	const FString URL = LavaPitMapURL.IsEmpty()
		? TEXT("/Game/Levels/L1_LavaPit?listen")
		: LavaPitMapURL;

	if (HasAuthority())
	{
		UE_LOG(Logminijam, Warning, TEXT("EL HOST COMNEZO LA PARTIDA, %s"), *URL);
		TravelToMap(URL);
	}
	else
	{
		UE_LOG(Logminijam, Warning, TEXT("CLIENTE QUIERE COMENZAR LA PARTIDA → PIDIENDO AL HOST %s"), *URL);
		Server_StartMatch(URL);
	}
}

void AminijamPlayerController::Server_StartMatch_Implementation(const FString& MapURL)
{
	TravelToMap(MapURL);
}

void AminijamPlayerController::TravelToMap(const FString& MapURL)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (World->IsNetMode(NM_ListenServer) || World->IsNetMode(NM_DedicatedServer))
	{
		World->ServerTravel(MapURL /* bAbsolute = false */);
	}
	else
	{
		// PRIMER NIVEL
		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/L1_LavaPit")));
	}
}
