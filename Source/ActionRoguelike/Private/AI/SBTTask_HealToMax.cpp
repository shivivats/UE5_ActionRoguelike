// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealToMax.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "SAttributeComponent.h"

USBTTask_HealToMax::USBTTask_HealToMax()
{
	HealTo = 0.8f;
}

EBTNodeResult::Type USBTTask_HealToMax::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
		if(AIPawn == nullptr)
			return EBTNodeResult::Failed;

		USAttributeComponent* AIAttributeComponent = USAttributeComponent::GetAttributeComp(AIPawn);
		if(AIAttributeComponent == nullptr)
			return EBTNodeResult::Failed;

		if(AIAttributeComponent->ApplyHealthChange(AIPawn, AIAttributeComponent->GetMaxHealth() * HealTo - AIAttributeComponent->GetHealth()))
		{
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

