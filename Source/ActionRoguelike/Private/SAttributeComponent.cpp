// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"),
	1.0f,
	TEXT("Multiplies ALL damage numbers. Default 1.0."),
	ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	MaxHealth = 100;

	HasRage = false;
	Rage = 100;
	MaxRage = 100;

	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const
{
	return Health >= MaxHealth;
}

bool USAttributeComponent::IsLowHealth() const
{
	return Health <= MaxHealth / 4;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->HasAuthority())
	{
		return false;
	}
	
	if (!GetOwner()->CanBeDamaged() && Delta < 0)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float PrevHealth = Health;
	float NewHealth = FMath::Clamp<float>(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = NewHealth - PrevHealth;
	
	
	Health = NewHealth;
		
	if (ActualDelta != 0)
	{
		// OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
		MulticastHealthChanged(InstigatorActor, Health, Delta);
	}

	if (HasRage)
	{
		const auto GainedRage = abs(ActualDelta);
		ApplyRageChange(InstigatorActor, GainedRage);
	}

	if (Health == 0 && ActualDelta != 0) // Died
		{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
		}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->HasAuthority())
	{
		return false;
	}
	
	Rage = FMath::Clamp<float>(Rage + Delta, 0.0f, MaxRage);
	MulticastRageChanged_Implementation(InstigatorActor, Rage, Delta);

	return Delta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(
			FromActor->GetComponentByClass(StaticClass()));
	}
	
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (USAttributeComponent* Ac = GetAttributes(Actor))
	{
		return Ac->IsAlive();
	}
	
	return false;
}

bool USAttributeComponent::IsActorLowHealth(AActor* Actor)
{
	if (USAttributeComponent* Ac = GetAttributes(Actor))
	{
		return Ac->IsLowHealth();
	}

	return false;
}


void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, MaxRage);

	// DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);	
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* Instigator, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(Instigator, this, NewRage, Delta);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth,
                                                                 float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta);
}
