// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	PlayerCredits = StartingCredits;
}

bool ASPlayerState::ModifyCredits(int32 Delta)
{
	if (PlayerCredits <= 0 && Delta < 0)
	{
		PlayerCredits = 0;
		return false;
	}

	PlayerCredits += Delta;

	OnCreditsChanged.Broadcast(this, PlayerCredits, Delta);
	return true;
}

bool ASPlayerState::AddCredits(int32 Amount)
{
	return ModifyCredits(Amount);
}

bool ASPlayerState::RemoveCredits(int32 Amount)
{
	if (PlayerCredits < Amount)
	{
		return false;
	}
	return ModifyCredits(-Amount);
}

int32 ASPlayerState::GetPlayerCredits() const
{
	return PlayerCredits;
}
