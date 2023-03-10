// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_HealToMax.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTask_HealToMax : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USBTTask_HealToMax();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	float HealAmount;

	//FTimerHandle TimerHandle_HealSelf;

	//UFUNCTION()
	//bool HealSelf(UBehaviorTreeComponent& OwnerComp);
	
};
