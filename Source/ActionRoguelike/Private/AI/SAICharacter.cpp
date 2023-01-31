// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParameterName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor); // if someone damaged the AI then it sets them as our target
		}

		// spawn widget on damage taken if widget doesn't already exist
		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass); // set world as the owner here, bc that's a suitable one that works for us
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;

				ActiveHealthBar->AddToViewport();
			}
		}
		// flash the ai character on hit just like the player
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameterName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f) // now we have just died
		{
			// stop behaviour tree
			//AAIController* AIC = Cast<AAIController>(GetController());
			AAIController* AIC = GetController<AAIController>();
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed"); // brain component is the superclass of behaviour tree basically
			}

			// ragdoll
			// since this is a skeletal mesh, we set all bones to start simulating physics, 
			// hence its now affected by gravity and the character ragdolls
			// but we keep in mind that we need to change the Collision profile of the mesh to be able to use physical collision, otherwise he falls through the floor
			GetMesh()->SetAllBodiesSimulatePhysics(true); 
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// set lifespan to destroy this actor
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	//AAIController* AIC = Cast<AAIController>(GetController());
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
