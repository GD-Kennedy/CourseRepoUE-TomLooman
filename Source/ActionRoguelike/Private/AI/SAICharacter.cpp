// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChange);
}

void ASAICharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                   float Delta)
{
	if (NewHealth <= 0.0f)
	{		
		if (Delta != 0.0f)
		{
			ASAIController* Aic = Cast<ASAIController>(GetController());
			if (Aic)
			{
				Aic->GetBrainComponent()->StopLogic("Killed");
				GetMesh()->SetAllBodiesSimulatePhysics(true);
				GetMesh()->SetCollisionProfileName("Ragdoll");
				SetLifeSpan(10.0f);
				
				if (InstigatorActor)
				{
					// UE::Math::TVector<double> PulseDir = (InstigatorActor->GetActorLocation() - GetActorLocation()) * Delta * 1000;
				}
			}
			else
			{
				SetLifeSpan(1.0f);
			}
		}
	}
	else if (Delta < 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FlashTimeToHitParam, GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedParam, HitFlashSpeedValue);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParam, HitFlashColorValue);

		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
	}
}

void ASAICharacter::OnPawnSeen(APawn* pawn)
{
	SetTargetActor(pawn);
}

void ASAICharacter::SetTargetActor(AActor* newTarget)
{
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller)
	{
		controller->GetBlackboardComponent()->SetValueAsObject("TargetActor", newTarget);
	}
}

