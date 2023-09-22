// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
	{
		if (USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Character))
		{
			if (AttributeComp->IsMaxHealth())
			{
				return;
			}
			
			ASPlayerState* PlayerState = Cast<ASPlayerState>(Character->GetPlayerState());
			const int32 CreditsBefore = PlayerState->GetPlayerCredits();
			if (!PlayerState->RemoveCredits(PickupCost))
			{
				UE_LOG(LogTemp, Log,
					   TEXT("Pickup attempt failed: HAD %i, COST %i, NOW %i"),
					   CreditsBefore, PickupCost, PlayerState->GetPlayerCredits());
				return;
			}
	
			UE_LOG(LogTemp, Log,
				   TEXT("Pickup attempt success: HAD %i, COST %i, NOW %i"),
				   CreditsBefore, PickupCost, PlayerState->GetPlayerCredits());
			
			Super::Interact_Implementation(InstigatorPawn);
			AttributeComp->ApplyHealthChange(this, 50.0f);
		}
	}
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if (USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (AttributeComp->IsMaxHealth())
		{
			return NSLOCTEXT(LOCTEXT_NAMESPACE, "Potion_FullHealthWarning", "Already at full health");
		}
	}

	return FText::Format(LOCTEXT("Potion_InctractMessage", "Cost {0} Credits - restores HP to maximum."), PickupCost);
}
#undef LOCTEXT_NAMESPACE