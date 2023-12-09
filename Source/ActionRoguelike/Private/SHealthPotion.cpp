// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SInteractable.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
	CreditCost = 50;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	// heal the player but only if the health is less than max
	// and if the player has enough credits

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp))
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PlayerState && PlayerState->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, HealAmount))
		{
			StartReactivationTimer();
		}
	}
}