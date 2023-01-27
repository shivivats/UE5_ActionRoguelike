// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractable.h"
#include "SPickupBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickupBase : public ASInteractable
{
	GENERATED_BODY()

public:
	ASPickupBase();

protected:
	
	ECollisionEnabled::Type OriginalCollisonType;

	// how much time after reactivate pickup
	UPROPERTY(EditDefaultsOnly)
	float ReactivationDelay;

	// Timer to reactivate the pickups
	FTimerHandle TimerHandle_PickupReactivation;

	// starts the reactivation timer
	void StartReactivationTimer();

	// this fn executes after the pickup timer elapses to reactivate the pickup
	void ReactivatePickup();

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
