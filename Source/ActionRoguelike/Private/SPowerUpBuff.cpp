// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBuff.h"
#include "SCharacter.h"
#include "GameActionSystem/SActionComponent.h"

void ASPowerUpBuff::Interact_Implementation(APawn* InstigatorPawn)
{
	if (GrantedAction)
	{
		USActionComponent* Action = Cast<ASCharacter>(InstigatorPawn)->GetActionComponent();
		if (Action && !Action->HasAction(GrantedAction))
		{
			Action->AddAction(InstigatorPawn, GrantedAction);
			Super::Interact_Implementation(InstigatorPawn);
		}
	}
}
