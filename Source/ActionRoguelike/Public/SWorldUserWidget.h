// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 * Our base class for widgets that display in various parts of the world
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// this pointer will be filled automatically so long as we have a matching variable name in the UI designer
	UPROPERTY(meta =(BindWidget)) 
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	// offset so we can properly display this above the AI characters
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	// this is the actor that the widget will be attached to
	// its important to mark this as a UPROPERTY so it works properly with unreal's framework
	UPROPERTY(BlueprintReadOnly, Category="UI")
	AActor* AttachedActor; 
};
