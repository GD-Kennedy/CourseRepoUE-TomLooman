// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSelfHealComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USSelfHealComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USSelfHealComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Heal")
	float SelfHealAmount;

public:
	UFUNCTION()
	float GetSelfHealAmount();
};
