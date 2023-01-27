// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SInteractable.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	// heal the player but only if the health is less than max

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(AttributeComp)
	{
		if(AttributeComp->ApplyHealthChange(HealAmount))
		{
			StartReactivationTimer();
		}
	}
}