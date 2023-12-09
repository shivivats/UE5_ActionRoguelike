// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits = 10.f;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// not enough credits
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

	return true;
}

float ASPlayerState::GetCredits()
{
	return Credits;
}