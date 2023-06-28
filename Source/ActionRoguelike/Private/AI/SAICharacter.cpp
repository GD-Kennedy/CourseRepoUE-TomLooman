// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp"); 
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* pawn)
{
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller)
	{
		UBlackboardComponent* bb = controller->GetBlackboardComponent();
		bb->SetValueAsObject("TargetActor", pawn);

		DrawDebugString(GetWorld(), GetActorLocation(),
			"PLAYER_SPOTTED", nullptr,
			FColor::White, 4.0f, true);
	}
}

