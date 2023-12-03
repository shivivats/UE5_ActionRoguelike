// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // make sure the AI gets a controller assigned to it by default

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::SetTargetActor(AActor* NewActor)
{
	ASAIController* AIC = Cast<ASAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewActor);
	}

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
	if (Delta < 0.0f) // if took damage
	{

		if (InstigatorActor == this)
		{
			SetTargetActor(InstigatorActor); // if we got hit by another actor (even an AI), we set the target to that
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		if (!ActiveHealthBar)
		{

			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		if (NewHealth <= AttributeComp->GetMinHealth()) // if dead
		{
			// stop Behaviour Tree
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				// brain component is like the parent of the behaviour tree
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll: we set all of the mesh's bones to simulate physies and thus they're pulled down by gravity and ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			// theres an issue here with the collision profile because the character mesh collision profile does not interact with collision (only queries)
			// so we need to change the collision profile here to the Ragdoll collision profile
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan so we show the ragdoll for a bit before deleting
			SetLifeSpan(10.0f);
		}
	}
}
