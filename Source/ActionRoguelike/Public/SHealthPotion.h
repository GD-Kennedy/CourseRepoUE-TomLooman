// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicPickup.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASBasicPickup
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
