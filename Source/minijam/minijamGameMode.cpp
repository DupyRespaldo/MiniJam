// Copyright Epic Games, Inc. All Rights Reserved.

#include "minijamGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AminijamGameMode::AminijamGameMode()
{

}


void AminijamGameMode::CompleteMission(APawn* Pawn)
{
	if (Pawn == nullptr) return;
	//Pawn->DisableInput(nullptr);
	if (SpectatorViewClass)
	{
		TArray<AActor*> ReturnActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatorViewClass, ReturnActors);
		if (ReturnActors.Num() > 0)
		{
			AActor* SpectatorActor = ReturnActors[0];
			for (FConstPlayerControllerIterator It=GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PC = It->Get();
				if (PC)
				{
					PC->SetViewTargetWithBlend(SpectatorActor,1.0f,VTBlend_Cubic);
				}
			}

		}
	}
	OnMissionComplete(Pawn);
}
