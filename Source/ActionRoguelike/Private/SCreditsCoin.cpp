// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditsCoin.h"
#include "SCharacter.h"

void ASCreditsCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
	{
		Character->GetPlayerState<ASPlayerState>()->AddCredits(1);
		Super::Interact_Implementation(InstigatorPawn);
	}
}