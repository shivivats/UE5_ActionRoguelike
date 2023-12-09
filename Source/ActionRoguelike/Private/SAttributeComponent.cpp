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
	// if heal and health already more than max, then no health change
	if (Delta > 0 && Health >= HealthMax)
	{
		return false;
	}

	// if damage and health already less than min, then no health change
	if (Delta < 0 && Health <= HealthMin)
	{
		return false;
	}

	Health += Delta;

	Health = FMath::Clamp(Health, HealthMin, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetHealthMin()
{
	return HealthMin;
}
