// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
	{
		if (USAttributeComponent* attributeComp = USAttributeComponent::GetAttributes(Character))
		{
			if (attributeComp->IsMaxHealth())
			{
				return;
			}
			Super::Interact_Implementation(InstigatorPawn);
			attributeComp->ApplyHealthChange(this, 50.0f);
		}
	}
}
