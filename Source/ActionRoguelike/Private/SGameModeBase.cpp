// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("su.SpawnBots"),
	true,
	TEXT("Enable spawning of bots via timer"),
	ECVF_Cheat);

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