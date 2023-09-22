// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	Credits = StartingCredits;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, StartingCredits);
	DOREPLIFETIME(ASPlayerState, Credits);
}

bool ASPlayerState::ModifyCredits(int32 Delta)
{
	if (!Owner->HasAuthority())
	{
		return false;
	}
	
	if (Credits <= 0 && Delta < 0)
	{
		Credits = 0;
		return false;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, GetPlayerId(), Credits, Delta);
	
	FString ActionMsg = FString::Printf(TEXT("[%d] Credits %s"),
				GetPlayerId(),
				*FString::FromInt(Credits));
	LogOnScreen(this, ActionMsg, FColor::White, 2.0f);
	return true;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	FString ActionMsg = FString::Printf(TEXT("[%d] Credits %s"),
				GetPlayerId(),
				*FString::FromInt(Credits));
	LogOnScreen(this, ActionMsg, FColor::White, 2.0f);
	OnCreditsChanged.Broadcast(this, GetPlayerId(), Credits, Credits - OldCredits);
}

bool ASPlayerState::AddCredits(int32 Amount)
{
	return ModifyCredits(Amount);
}

bool ASPlayerState::RemoveCredits(int32 Amount)
{
	if (Credits < Amount)
	{
		return false;
	}
	return ModifyCredits(-Amount);
}

int32 ASPlayerState::GetPlayerCredits() const
{
	return Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->CreditsPerPlayerData.Add(GetPlayerId()) = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		TMap<int32, int32> CreditsData = SaveObject->CreditsPerPlayerData;
		if (CreditsData.Contains(GetPlayerId()))
		{
			Credits = SaveObject->CreditsPerPlayerData[GetPlayerId()];
		}
		else
		{
			Credits = 0;
		}
	}
}
