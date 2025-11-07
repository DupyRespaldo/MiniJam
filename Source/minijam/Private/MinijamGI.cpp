#include "MinijamGI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UMinijamGI::Lobby_TryJoinLocalOrHost()
{
	if (!GetWorld()) return;

	ENetMode NM = GetWorld()->GetNetMode();


	if (NM == NM_ListenServer || NM == NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GI] ListenServer → OpenLevel %s ?listen"), *LobbyMap);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LobbyMap), /*bAbsolute=*/true, TEXT("listen"));
		return;
	}

	if (NM == NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("ESPERANDO AL HOST QUE VIAJE AL MAPA"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT(" %s, FALLBACK A HOST"), *HostAddress);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		// join al host si existe (si no, fallará ESO NO FUNCIONA Y CRASHEA EL JUEGO)
		PC->ClientTravel(HostAddress, TRAVEL_Absolute);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		JoinTimeoutHandle, this, &UMinijamGI::TryHostFallback, JoinFallbackDelay, false);
}

void UMinijamGI::TryHostFallback()
{
	if (!GetWorld()) return;

	const FString CurrentMap = GetWorld()->GetMapName();
	if (CurrentMap.Contains(TEXT("IL_Lobby")))
	{
		UE_LOG(LogTemp, Warning, TEXT("ESTAMOS EN EL LOBBY"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[GI] Fallback HOST → OpenLevel %s ?listen"), *LobbyMap);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LobbyMap), /*bAbsolute=*/true, TEXT("listen"));
}

void UMinijamGI::Shutdown()
{
	ClearJoinTimer();
	Super::Shutdown();
}

void UMinijamGI::ClearJoinTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(JoinTimeoutHandle);
	}
}

