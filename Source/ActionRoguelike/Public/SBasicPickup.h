// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SBasicPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBasicPickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASBasicPickup();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

protected:
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_Cooldown;
	
protected:
	virtual void BeginPlay() override;
};
