// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupBase.h"
#include "SInteractable.h"
#include "Components/StaticMeshComponent.h"

ASPickupBase::ASPickupBase()
{	
	ReactivationDelay = 10.0f;
}

void ASPickupBase::StartReactivationTimer()
{
	// make the mesh invisible
	BaseMesh->SetVisibility(false);
	
	// make it not handle collisions anymore
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// set the inactivation timer and tell it to call reactivate pickup function
	GetWorldTimerManager().SetTimer(TimerHandle_PickupReactivation, this, &ASPickupBase::ReactivatePickup, ReactivationDelay);
}

void ASPickupBase::ReactivatePickup()
{
	// make the mesh visible again 
	BaseMesh->SetVisibility(true);

	// make the mesh handle collisions again
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
}

