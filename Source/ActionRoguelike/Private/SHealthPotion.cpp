// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* character = Cast<ASCharacter>(InstigatorPawn))
	{
		if (USAttributeComponent* attributeComp = Cast<USAttributeComponent>(
			character->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			if (attributeComp->IsMaxHealth())
			{
				return;
			}
			Super::Interact_Implementation(InstigatorPawn);
			attributeComp->ApplyHealthChange(50.0f);
		}
	}
}
