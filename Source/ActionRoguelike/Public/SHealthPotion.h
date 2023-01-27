// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickupBase
{
	GENERATED_BODY()

public:
	ASHealthPotion();

protected:

	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
