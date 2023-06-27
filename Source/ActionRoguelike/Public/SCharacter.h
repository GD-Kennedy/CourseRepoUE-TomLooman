// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";
	
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
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* CastingVFX;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileBPClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void StartProjectileAction(TSubclassOf<AActor> projectile, float delay);
	void PrimaryAttack_TimeElapsed();
	void SpecialAttack();
	void SpecialAttack_TimeElapsed();
	void SpawnProjectile(TSubclassOf<AActor> projectileType);

	void PrimaryInteract();
	void Dash();
	void Dash_TimeElapsed();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UCameraComponent* GetCamera()
	{
		return CameraComponent;
	}

	UFUNCTION()
	void PlayerTeleport(FVector position, FRotator rotation);
};
