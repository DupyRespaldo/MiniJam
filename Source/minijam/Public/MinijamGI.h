#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MinijamGI.generated.h"

UCLASS()
class UMinijamGI : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Lobby_TryJoinLocalOrHost();

	virtual void Shutdown() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LobbyMap = TEXT("/Game/Levels/IL_Lobby");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString HostAddress = TEXT("127.0.0.1");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float JoinFallbackDelay = 1.5f;

private:
	void TryHostFallback();
	void ClearJoinTimer();

	FTimerHandle JoinTimeoutHandle;
};
