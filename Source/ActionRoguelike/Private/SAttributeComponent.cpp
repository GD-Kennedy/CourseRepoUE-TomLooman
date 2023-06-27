// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	MaxHealth = 100;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const
{
	return Health >= MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{	
	if (Health < Delta)
	{
		Delta = Health;
	}
	Health += Delta;
	Health = FMath::Clamp<float>(Health, 0.0f, MaxHealth);
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}
