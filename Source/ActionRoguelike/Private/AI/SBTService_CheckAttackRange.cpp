// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	if (!(ensure(bb)))
	{
		return;
	}
	
	AAIController* controller = OwnerComp.GetAIOwner();
	if (!(ensure(controller)))
	{
		return;
	}
	
	AActor* target = Cast<AActor>(bb->GetValueAsObject(TargetActor.SelectedKeyName));
	if (!target)
	{
		return;
	}
	
	APawn* pawn = controller->GetPawn();
	if (ensure(pawn))
	{
		double distance = FVector::Distance(target->GetActorLocation(), pawn->GetActorLocation());
		bb->SetValueAsBool(AttackRangeKey.SelectedKeyName,
			distance < 2000.f && controller->LineOfSightTo(target));
	}
}
