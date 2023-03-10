// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100.f;

	HealthMax = 100.f;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -1 * GetHealthMax());
}

bool USAttributeComponent::IsLowHealth(float LowHealthThreshold)
{
	return (Health<=LowHealthThreshold);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	// if the owner can't be damaged then return false (this implementation is available in actor by default)
	if(!GetOwner()->CanBeDamaged()) 
	{
		return false;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, HealthMin, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

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


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);

	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false; // default response is dead
}
