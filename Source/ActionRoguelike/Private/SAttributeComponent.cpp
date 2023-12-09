// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100.f;

	HealthMax = 100.f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, HealthMin, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	// if the actual delta is 0 then we had no change so we return false
	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealthMin() const
{
	return HealthMin;
}
