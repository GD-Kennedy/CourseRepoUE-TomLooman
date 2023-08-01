// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "SSelfHealComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}
	
	auto healComp = Cast<USSelfHealComponent>(Pawn->GetComponentByClass(USSelfHealComponent::StaticClass()));
	if (!healComp)
	{
		return EBTNodeResult::Failed;
	}
	
	auto healValue = healComp->GetSelfHealAmount();
	if (healValue > 0)
	{
		USAttributeComponent::GetAttributes(Pawn)->ApplyHealthChange(Pawn, healValue);
	}
	return EBTNodeResult::Succeeded;
}
