// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SInteractable.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASInteractable : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASInteractable();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

};
