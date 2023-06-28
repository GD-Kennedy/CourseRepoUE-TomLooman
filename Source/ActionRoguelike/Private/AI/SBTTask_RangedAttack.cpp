// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	if (!ensure(controller))
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	if (!ensure(bb))
	{
		return EBTNodeResult::Failed;
	}
	
	ASAICharacter* pawn = Cast<ASAICharacter>(controller->GetPawn());
	if (pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector spawnLocation = pawn->GetMesh()->GetSocketLocation("Muzzle_01");
	
	AActor* target = Cast<AActor>(bb->GetValueAsObject("TargetActor"));
	if (target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector direction = target->GetActorLocation() - spawnLocation;
	FRotator spawnRotation = direction.Rotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	pawn->SetActorRotation(spawnRotation);
	
	ASBasicProjectile* projectile =
		GetWorld()->SpawnActor<ASBasicProjectile>(ProjectileClass, spawnLocation, spawnRotation, params);

	return projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
