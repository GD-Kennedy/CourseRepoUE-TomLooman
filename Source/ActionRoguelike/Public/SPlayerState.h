// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, PlayerId, int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

	ASPlayerState();

public:	
	UFUNCTION(BlueprintCallable, Category="Credits")
	bool AddCredits(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	bool RemoveCredits(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	int32 GetPlayerCredits() const;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

protected:
	bool ModifyCredits(int32 Delta);

protected:
	UPROPERTY(EditDefaultsOnly, Replicated, SaveGame, Category="Credits")
	int32 StartingCredits;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_Credits", SaveGame, Category="Credits")
	int32 Credits;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
};
