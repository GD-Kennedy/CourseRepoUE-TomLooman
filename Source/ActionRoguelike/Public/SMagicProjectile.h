// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicProjectile.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
 
UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBasicProjectile
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;
};
