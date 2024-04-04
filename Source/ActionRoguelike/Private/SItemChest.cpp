// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "SInteractable.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;

	SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	bLidOpened = !bLidOpened;

	OnRep_LidOpened();
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();

}

void ASItemChest::OnRep_LidOpened()
{
	// this fn is automatically called whenever the bLidOpened value is updated.
	// but RepNotify's are only triggered for clients, so we need to manually call it for servers

	float CurrentPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// now we set the rules on the replicated props
	// DOREPLIFETIME is a macro
	// requires a class, and a variable from that class
	// this indicates unreal to simply replicate this variable to all clients (most straightforward way)
	DOREPLIFETIME(ASItemChest, bLidOpened);
}