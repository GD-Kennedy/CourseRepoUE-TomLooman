// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageSoure, AActor* TargetActor, float DamageAmount)
{
	if (USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor))
	{
		return AttributeComp->ApplyHealthChange(DamageSoure, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageSoure, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageSoure, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.Location, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
