// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{

	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	// Placeholder code: manually grab the player character and put his location in here
	// We'll update this to pawn sensing later

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(MyPawn)
	{
		// Later expose the KeyName string to blueprnt as well so we can change it more easily
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);

	}
	
}
