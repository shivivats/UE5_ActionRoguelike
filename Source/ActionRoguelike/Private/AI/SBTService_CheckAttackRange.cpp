// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check distance between AI Pawn and Target Actor
	// (optionally also line of sight checks)

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if(ensure(BlackboardComp)) // we expect to always have a blackboard component with the behaviour tree
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if(ensure(MyController)) // we expect the behaviour tree to always be associated with a controller
			{
				APawn* AIPawn = MyController->GetPawn();
				if(ensure(AIPawn)) // if we're running the behaviour tree, the AI Pawn should always exist
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.f;

					bool bHasLOS = false;
					if(bWithinRange) // only check LOS when within range
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor); // does a line trace against channel visibility
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
