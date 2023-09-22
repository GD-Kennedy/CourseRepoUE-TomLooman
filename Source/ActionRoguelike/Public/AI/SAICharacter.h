// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "GameActionSystem/SActionComponent.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class UPawnSensingComponent;
class SUserWidget;
class USWorldUserWidget;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

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

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> EnemyDetectedWidgetClass;

	UPROPERTY()
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(Replicated)
	USWorldUserWidget* EnemyDetectedMark;
	
	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
	
	void ShowEnemyDetectedWidget();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastShowDetectedMark();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	void HandleDeath(float Delta);

	void HandleDamage(AActor* InstigatorActor);
	void InitializeHealthBar();
	void ToggleRagdollPhysics();
	void FlashDamageOnHit();
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
};
