// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"

void ASBasicPickup::Interact_Implementation(APawn* InstigatorPawn)
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
			
			attributeComp->ApplyHealthChange(50.0f);

			MeshComp->SetVisibility(false);
			auto prevCol = MeshComp->GetCollisionEnabled();
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, [=]			{
				MeshComp->SetVisibility(true);
				MeshComp->SetCollisionEnabled(prevCol);
			}, 10.0f, false);
		}
	}
}
