// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskRangedAttack.generated.h"

/**
 *
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTaskRangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	USBTTaskRangedAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MaxBulletSpreadPitch;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MaxBulletSpreadYaw;
};
