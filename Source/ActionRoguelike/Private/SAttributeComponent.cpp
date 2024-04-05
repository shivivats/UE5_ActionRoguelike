// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat); //marking it as a cheat wont include it in the final build which is good for testing purposes


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;

	SetIsReplicatedByDefault(true);

}

USAttributeComponent* USAttributeComponent::GetAttributeComp(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass((USAttributeComponent::StaticClass())));
	}

	return nullptr;

}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributeComp(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) // CanBeDamaged is a god mode check
	{
		return false;
	}

	if (Delta < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, HealthMin, HealthMax);

	float ActualDelta = Health - OldHealth;

	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	// only broadcast when health is actually changed.
	// this is a cpu and network consideration
	if (ActualDelta != 0.0f)
	{

		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetMinHealth()
{
	return HealthMin;
}

float USAttributeComponent::GetMaxHealth()
{
	return HealthMax;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	DOREPLIFETIME(USAttributeComponent, HealthMin);

	// Make it so that HealthMax is only replicated to the player that owns it.
	// This is just a bit of CPU and Network optimisation
	// Can also do COND_InitialOnly for values that will never change after initialisation
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMin, COND_OwnerOnly);

}