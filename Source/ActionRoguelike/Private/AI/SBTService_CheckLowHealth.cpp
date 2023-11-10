// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check our own health

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (ensure(AIController))
		{
			APawn* AIPawn = AIController->GetPawn();
			if (ensure(AIPawn))
			{
				USAttributeComponent* AIAttributeComponent = USAttributeComponent::GetAttributeComp(AIPawn);

				bool bShouldHeal = false;

				if (ensure(AIAttributeComponent))
				{
					if (AIAttributeComponent->GetHealth() <= 50.0f)
						bShouldHeal = true;
				}

				BlackboardComp->SetValueAsBool(ShouldHealKey.SelectedKeyName, bShouldHeal);
			}
		}
	}
}
