// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "GameActionSystem/SAction.h"
#include "SAction_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Thorns : public USAction
{
	GENERATED_BODY()


public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(EditDefaultsOnly, Category="Details");
	float PercentageReturned;
};
