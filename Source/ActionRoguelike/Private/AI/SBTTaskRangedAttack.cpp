// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskRangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTaskRangedAttack::USBTTaskRangedAttack()
{
	MaxBulletSpreadPitch = 2.0f;
	MaxBulletSpreadYaw = 2.0f;
}

EBTNodeResult::Type USBTTaskRangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
		if (AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}


		FVector ShootDirection = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = ShootDirection.Rotation();

		// add some random bullet spread
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpreadPitch);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpreadYaw, MaxBulletSpreadYaw);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AIPawn; // add the AI Pawn as instigator so the generated projectile doesnt hit the AI that spawns it

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;

}
