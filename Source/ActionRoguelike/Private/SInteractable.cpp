// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractable.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASInteractable::ASInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

void ASInteractable::Interact_Implementation(APawn* InstigatorPawn)
{

}

// Called when the game starts or when spawned
void ASInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

