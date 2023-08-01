// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPlayerState.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:		
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
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void SpecialAttack();

	void PrimaryInteract();
	void Dash();
	
	void SprintStart();
	void SprintStop();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnRageChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	virtual void PostInitializeComponents() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void PlayerTeleport(FVector Position, FRotator Rotation);

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
	
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetPawnViewRotation() const;
	UCameraComponent* GetCamera() const;
	USActionComponent* GetActionComponent() const;
};
