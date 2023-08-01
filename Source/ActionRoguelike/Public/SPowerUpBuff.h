// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicPickup.h"
#include "GameActionSystem/SAction.h"
#include "SPowerUpBuff.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUpBuff : public ASBasicPickup
{
	GENERATED_BODY()
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category="GrantedAction")
	TSubclassOf<USAction> GrantedAction;
};
