// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ASBlackholeProjectile::ASBlackholeProjectile()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ForceStrength = -1500000.0f;
	RadialForceComp->SetupAttachment(SphereComp);
	// set objects to affect in blueprint

	DestroySphereComp = CreateDefaultSubobject<USphereComponent>("DestroySphereComp");
	DestroySphereComp->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
	DestroySphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnBeginOverlap);
	DestroySphereComp->SetSphereRadius(100.0f);
	DestroySphereComp->SetupAttachment(SphereComp);
	// set navigation area class in blueprint

	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;

}

void ASBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.0f);

	// ignore the player character when moving so we dont end up destroying ourselves
	DestroySphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASBlackholeProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp->IsSimulatingPhysics() && OtherActor)
	{
		GetWorld()->DestroyActor(OtherActor);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactAudio, GetActorLocation());
	}
}
