// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicPickup.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	ASGameModeBase();
		
	virtual void StartPlay() override;

	UFUNCTION()
	virtual void OnActorKilled(AActor* KilledActor, AActor* Killer);

protected:
	FString SlotName = "SaveGame01";
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UClass* MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category="OnLevelStart")
	const UEnvQuery* EQS_FindPickupSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category="OnLevelStart")
	TMap<TSubclassOf<ASBasicPickup>, int32> PickupsToSpawnMap;

	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void SpawnItemsAtStart();
	
	UFUNCTION()
	void OnSpawnStartItemsQueryFinished(FEnvQueryResult Result);

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();
	
	void LoadSaveGame();
};
