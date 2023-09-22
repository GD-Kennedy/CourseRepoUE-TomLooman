// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicProjectile.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASBasicProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashProjectile();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Misc")
	float LifeSpan;
	FTimerHandle TimerHandle_DashDelay;
	
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void PostInitializeComponents() override;

private:
	void TriggerDelayedTeleport();
	virtual void BeginPlay() override;
};
