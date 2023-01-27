// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Explodes after 0.2 seconds

// Play particle effect at point of detonation

// Wait 0.2 seconds more before teleporting playercharacter (aka instigator of projectile)

// Let the detonation effect play before we teleport

// Make sure you ‘stop’ the projectile movement while you wait(!)

// Look back at ‘Instigator’ we used for ignoring collisions, and re-apply here for teleportation.

// On hit with world: immediately stop movement and execute the same behavior (explode + teleport)

ASTeleportProjectile::ASTeleportProjectile()
{
	DetonationDelay = 0.2f;

	TeleportationDelay = 0.2f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DetonationTimer, this, &ASTeleportProjectile::Detonate, DetonationDelay);
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnHit);
}

void ASTeleportProjectile::Detonate()
{
	MovementComp->StopMovementImmediately();

	EffectComp->SetVisibility(false);

	FVector SpawnLocation = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DetonationEffect, SpawnLocation);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportationTimer, this, &ASTeleportProjectile::TeleportInstigator, TeleportationDelay);
}

void ASTeleportProjectile::TeleportInstigator()
{
	FVector TeleportLocation = GetActorLocation();

	GetInstigator()->SetActorLocation(TeleportLocation);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactAudio, TeleportLocation);

	Destroy();
}

void ASTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DetonationTimer);

	Detonate();
}
