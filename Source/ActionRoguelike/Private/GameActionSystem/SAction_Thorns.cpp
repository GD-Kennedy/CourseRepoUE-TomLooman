// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActionSystem/SAction_Thorns.h"

#include "MathUtil.h"
#include "SAttributeComponent.h"
#include "GameActionSystem/SActionComponent.h"

void USAction_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(
		GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USAction_Thorns::OnHealthChanged);
	}
}

void USAction_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(
		GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USAction_Thorns::OnHealthChanged);
	}
}

void USAction_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp,
                                      float NewHealth, float Delta)
{
	if (Delta > 0)
	{
		return;
	}
	AActor* Owner = GetOwningComponent()->GetOwner();
	if (InstigatorActor == Owner)
	{
		return;
	}
	
	USAttributeComponent* InstigatorAttributes = USAttributeComponent::GetAttributes(
		InstigatorActor);
	if (InstigatorAttributes)
	{
		InstigatorAttributes->ApplyHealthChange(Owner, FGenericPlatformMath::RoundToInt(Delta * PercentageReturned / 100));
	}
}
