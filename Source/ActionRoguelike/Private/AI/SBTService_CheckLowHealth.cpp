// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Bb = OwnerComp.GetBlackboardComponent();
	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Attributes = USAttributeComponent::GetAttributes(Pawn);
	if(ensure(Attributes))
	{
		Bb->SetValueAsBool(IsLowHealthKey.SelectedKeyName, Attributes->IsLowHealth());
	}
}