// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBasicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBasicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBasicProjectile();

	UParticleSystem* GetCastVFX()
	{
		return CastVFX;
	}

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystem* CastVFX;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* FlightSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* ImpactSound;
		
	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<UCameraShakeBase> CameraShakeComponent;
	
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
