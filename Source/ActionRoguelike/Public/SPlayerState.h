// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

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

protected:
	bool ModifyCredits(int32 Delta);
	
	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int32 StartingCredits;

	UPROPERTY(BlueprintReadOnly, Category="Credits")
	int32 PlayerCredits;
};
