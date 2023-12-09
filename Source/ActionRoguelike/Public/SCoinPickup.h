// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "SCoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoinPickup : public ASPickupBase
{
	GENERATED_BODY()
public:
	ASCoinPickup();
protected:


	UPROPERTY(EditAnywhere, Category="Credits")
	int32 CreditsAmount;

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
