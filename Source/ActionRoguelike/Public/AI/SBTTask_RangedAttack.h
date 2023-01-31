// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	
	USBTTask_RangedAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletPitchSpread; // max bullet spread in degrees

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletYawSpread; // max bullet spread in degrees

	// edit defaults only doesnt work here, for AI we always use edit anywhere
	UPROPERTY(EditAnywhere, Category="AI") 
	TSubclassOf<AActor> ProjectileClass;
};
