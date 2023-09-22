// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("su.SpawnBots"),
	true,
	TEXT("Enable spawning of bots via timer"),
	ECVF_Cheat);

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	
	
	if (ASPlayerState* PlayerState = Cast<ASPlayerState>(NewPlayer->PlayerState))
	{
		if (APlayerController* PC = Cast<APlayerController>(NewPlayer))
		{
			if (PC->IsLocalPlayerController())
			{
				UE_LOG(LogTemp, Warning, TEXT("This player is a client, id 1"));
				PlayerState->SetPlayerId(1);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("This player is the server, id 0"));
				PlayerState->SetPlayerId(0);
			}
		}
		
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	SpawnItemsAtStart();
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cheat."))
		return;
	}
	
	float MaxBotCount = 10; // Fallback
	if (ensure(DifficultyCurve))
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	int AliveBotsCount = 0;
	
	for (TActorIterator<ASAICharacter> Character(GetWorld()); Character; ++Character)
	{
		ASAICharacter* Bot = *Character;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AliveBotsCount++;
		}		
	}
	
	if (AliveBotsCount >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max bots. Skipping."))
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* Instance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(Instance))
	{
		Instance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status)
{
	if (Status != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS failed."))
		return;
	}

	TArray<FVector> Locations = Instance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		UE_LOG(LogTemp, Warning, TEXT("Bot spawned."))
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> Character(GetWorld()); Character; ++Character)
	{
		ASAICharacter* Bot = *Character;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: Pass player for credits?
		}		
	}
}

void ASGameModeBase::OnActorKilled(AActor* KilledActor, AActor* Killer)
{
	ACharacter* Killed = Cast<ACharacter>(KilledActor);
	if (!Killed)
	{
		return;
	}

	if (ASCharacter* Player = Cast<ASCharacter>(KilledActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(ASGameModeBase, RespawnPlayerElapsed), Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else if (Cast<ASAICharacter>(KilledActor))
	{
		ASCharacter* InstigatorPlayer = Cast<ASCharacter>(Killer);
		if (InstigatorPlayer)
		{
			ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPlayer->GetPlayerState());
			if (ensure(PlayerState))
			{
				PlayerState->AddCredits(1);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("[OnActorKilled] Victim: %s, Killed %s"), *GetNameSafe(KilledActor), *GetNameSafe(KilledActor))
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

// Define a function to run the query and spawn the items
void ASGameModeBase::SpawnItemsAtStart()
{
	FEnvQueryRequest QueryRequest = FEnvQueryRequest(EQS_FindPickupSpawnLocations, this);

	FQueryFinishedSignature QueryFinished;
	QueryFinished.BindLambda([this](TSharedPtr<FEnvQueryResult> Result)
	{
		OnSpawnStartItemsQueryFinished(*Result);
	});
	QueryRequest.Execute(EEnvQueryRunMode::SingleResult, QueryFinished);
}

void ASGameModeBase::OnSpawnStartItemsQueryFinished(FEnvQueryResult Result)
{
	if (Result.IsFinished())
	{
		TArray<FVector> OutLocations;
		Result.GetAllAsLocations(OutLocations);
		
		for(const TTuple<TSubclassOf<ASBasicPickup>, int>& Pickup : PickupsToSpawnMap)
		{
			for(int32 i = 0; i < Pickup.Value; ++i)
			{
				if(OutLocations.Num() == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("OutLocations array is empty, can't spawn more pickups"));
					return;
				}

				int32 RandomIndex = FMath::RandRange(0, OutLocations.Num() - 1);

				FVector SpawnLocation = OutLocations[RandomIndex];
				OutLocations.RemoveAt(RandomIndex);

				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<ASBasicPickup>(Pickup.Key, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
}

void ASGameModeBase::WriteSaveGame()
{
	TArray<TObjectPtr<APlayerState>> PlayerArray = GameState->PlayerArray;
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PlayerArray[i]))
		{
			FString PlayerSlotName = SlotName + FString::FromInt(PlayerState->GetUniqueID());

			PlayerState->SavePlayerState(CurrentSaveGame);
			
			CurrentSaveGame->SavedActors.Empty();
			
			for (FActorIterator It(GetWorld()); It; ++It)
			{
				AActor* Actor = *It;
				if (!Actor->Implements<USGameplayInterface>())
				{
					continue;
				}

				FActorSaveData ActorData;
				ActorData.ActorName = Actor->GetName();
				ActorData.Transform = Actor->GetTransform();

				// Pass the array to fill with data from Actor
				FMemoryWriter MemoryWriter(ActorData.ByteData);
		
				FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
				// Find only vars with UPROPERTY(SaveGame)
				Ar.ArIsSaveGame = true;
				//Convert's SaveGame UPROPERTIES into binary array
				Actor->Serialize(Ar);
		
				CurrentSaveGame->SavedActors.Add(ActorData);
			}
			UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
			UE_LOG(LogTemp, Log, TEXT("Saved SaveGame Data for %s"), *PlayerSlotName);
		}
	}
}

void ASGameModeBase::LoadSaveGame()
{
	// Doesnt work with multiplayer - requires rework of some sort
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."))
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."))
		}
		
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemoryReader(ActorData.ByteData);
		
					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
					Ar.ArIsSaveGame = true;
					// Convert's binary array back to SaveGame 
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."))
	}
}