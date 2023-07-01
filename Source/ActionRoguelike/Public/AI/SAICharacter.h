// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "On hit params")
	FName FlashTimeToHitParam = "HitFlashTime";

	UPROPERTY(EditAnywhere, Category = "On hit params")
	FName HitFlashSpeedParam = "HitFlashSpeed";
	
	UPROPERTY(EditAnywhere, Category = "On hit params")
	float HitFlashSpeedValue = 1.0f;

	UPROPERTY(EditAnywhere, Category = "On hit params")
	FName HitFlashColorParam = "HitFlashColor";
	
	UPROPERTY(EditAnywhere, Category = "On hit params")
	FVector4 HitFlashColorValue;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* newTarget);

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* pawn);
};
