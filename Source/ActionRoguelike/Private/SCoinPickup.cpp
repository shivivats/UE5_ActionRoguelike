// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickup.h"

#include "SPlayerState.h"

ASCoinPickup::ASCoinPickup()
{
	CreditsAmount = 20;
}

void ASCoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(!ensure(InstigatorPawn))
	{
		return;
	}

	ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());

	if(ensure(PlayerState))
	{
		PlayerState->AddCredits(CreditsAmount);
		StartReactivationTimer();
	}
}
