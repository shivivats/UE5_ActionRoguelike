// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealToMax.h"
#include <AIModule/Classes/AIController.h>
#include "SAttributeComponent.h"
#include <GameFramework/Character.h>
#include <AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>

USBTTask_HealToMax::USBTTask_HealToMax()
{
	HealAmount = 10.0f;
}


EBTNodeResult::Type USBTTask_HealToMax::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// heal back to max hit points
	// this task will be run after the player has already found a safe spot

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComp = MyPawn->FindComponentByClass<USAttributeComponent>();
		if (ensure(AttributeComp))
		{
			//if (AttributeComp->ApplyHealthChange(MyPawn, HealAmount))
			if (AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax()))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}