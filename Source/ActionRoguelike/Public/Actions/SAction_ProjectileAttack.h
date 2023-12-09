// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class ASProjectile;
class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* ProjectileAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ProjectileSpawnEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ProjectileAttackSpawnDelay;

	FTimerHandle TimerHandle_ProjectileAttack;

	UFUNCTION()
	void ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter);
};
