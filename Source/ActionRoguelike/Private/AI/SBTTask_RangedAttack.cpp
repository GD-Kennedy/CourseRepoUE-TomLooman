// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!ensure(Controller))
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!ensure(BB))
	{
		return EBTNodeResult::Failed;
	}
	
	ASAICharacter* Pawn = Cast<ASAICharacter>(Controller->GetPawn());
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
		
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (!USAttributeComponent::IsActorAlive(Target))
	{
		return EBTNodeResult::Failed;
	}
	
	FVector SpawnLocation = Pawn->GetMesh()->GetSocketLocation("Muzzle_01");
	
	FVector Direction = Target->GetActorLocation() - SpawnLocation;
	FRotator SpawnRotation = Direction.Rotation();

	SpawnRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
	SpawnRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = Pawn;
	
	Pawn->SetActorRotation(SpawnRotation);
	
	ASBasicProjectile* projectile =
		GetWorld()->SpawnActor<ASBasicProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);

	return projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
