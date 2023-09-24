// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}

void ASAICharacter::MulticastShowDetectedMark_Implementation()
{
	ShowEnemyDetectedWidget();
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChange);
}

void ASAICharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f)
	{
		HandleDeath(Delta);
	}
	else if (Delta < 0)
	{
		HandleDamage(InstigatorActor);
	}
}

void ASAICharacter::HandleDeath(float Delta)
{
	if (Delta != 0.0f)
	{
		ASAIController* Aic = Cast<ASAIController>(GetController());
		if (Aic)
		{
			Aic->GetBrainComponent()->StopLogic("Killed");
			ToggleRagdollPhysics();
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10.0f);
		}
		else
		{
			SetLifeSpan(1.0f);
		}
	}
}

void ASAICharacter::HandleDamage(AActor* InstigatorActor)
{
	if (InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	InitializeHealthBar();	
	FlashDamageOnHit();
}

void ASAICharacter::InitializeHealthBar()
{
	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void ASAICharacter::ToggleRagdollPhysics()
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASAICharacter::FlashDamageOnHit()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FlashTimeToHitParam, GetWorld()->TimeSeconds);
	GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedParam, HitFlashSpeedValue);
	GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParam, HitFlashColorValue);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{	
	SetTargetActor(Pawn);
}

void ASAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASAICharacter, EnemyDetectedMark);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller)
	{
		if (controller->GetBlackboardComponent()->GetValueAsObject("TargetActor") != NewTarget)
		{
			if (Owner->HasAuthority())
			{
				MulticastShowDetectedMark();
			}
		}
		controller->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::ShowEnemyDetectedWidget()
{
	if (EnemyDetectedMark == nullptr)
	{
		EnemyDetectedMark = CreateWidget<USWorldUserWidget>(GetWorld(), EnemyDetectedWidgetClass);
		
		if (EnemyDetectedMark)
		{
			EnemyDetectedMark->AttachedActor = this;
			EnemyDetectedMark->AddToViewport();
		}
	}
}
