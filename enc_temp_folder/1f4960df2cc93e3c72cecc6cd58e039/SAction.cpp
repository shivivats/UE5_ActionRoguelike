// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running Action %s"), *GetNameSafe(this));

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping Action %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning); // assert to make sure that we're only calling StopAction when the action is running

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating this object using NewObject<T>
	UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (ActorComp)
	{
		return ActorComp->GetWorld();
	}

	return nullptr;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	USActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		// dont start this action if there's blocked tags
		return false;
	}

	return true;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning()
{
	return bIsRunning;
}

