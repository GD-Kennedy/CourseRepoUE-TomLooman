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

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;
	
	UFUNCTION()
	void TogglePowerUp(bool bNewIsActive);
	
	UFUNCTION()
	void OnRep_IsActive();

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 PickupCost;
	
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_Cooldown;
};
