// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include <AIModule/Classes/AIController.h>
#include <GameFramework/Character.h>
#include "SAttributeComponent.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthThreshold = 30.0f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check if the character is low health
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* MyController = OwnerComp.GetAIOwner();

	if(ensure(MyController) && ensure(BlackboardComp))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if(MyPawn)
		{
			USAttributeComponent* AttributeComp = MyPawn->FindComponentByClass<USAttributeComponent>();

			BlackboardComp->SetValueAsBool(CheckHealthKey.SelectedKeyName, AttributeComp->IsLowHealth(LowHealthThreshold));
		}
	}
}
