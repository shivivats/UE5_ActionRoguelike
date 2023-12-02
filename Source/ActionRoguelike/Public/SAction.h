// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

/**
 * 
 */
UCLASS(Blueprintable) // Add the keyword here otherwise we cant make child classes of this in BP
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY() 

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	/* Tags added to owning actor when action activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these tags applied */
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;

public:

	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override; // we override this to get access to it in blueprints
};
