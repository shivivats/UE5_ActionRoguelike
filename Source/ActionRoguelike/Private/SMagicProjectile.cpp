// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	DestroyDelay = 0.2f;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnBeginOverlap);
}

void ASMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			// get the movement comp and invert the projectile velocity
			MovementComp->Velocity = -MovementComp->Velocity;
			// also we said rotation follows velocity in the projectile base so we'll automatically turn the rotation as well with this

			SetInstigator(Cast<APawn>(OtherActor)); // set the parrying actor as the instigator otherwise this projectile would never hit the actor that shot it

			return;
		}

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		// Apply Damage & Impulse - use the static fn we have instead of just checking if AttributeComp is valid
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -1.0f * Damage);

			if(GetWorldTimerManager().IsTimerActive(TimerHandle_DestroyProjectile))
			{
				GetWorldTimerManager().ClearTimer(TimerHandle_DestroyProjectile);
			}

			GetWorldTimerManager().SetTimer(TimerHandle_DestroyProjectile, this, &ASMagicProjectile::DestroyProjectile, DestroyDelay);
		}

		DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Blue, 1.f, 0.f);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, SweepResult.ImpactPoint, GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactAudio, GetActorLocation());

		// shake the world - any cameras between 5 and 2000 away will be shaked
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShake, GetActorLocation(), 0.1f, 2000.0f);

		GetWorldTimerManager().SetTimer(TimerHandle_DestroyProjectile, this, &ASMagicProjectile::DestroyProjectile, DestroyDelay);
	}
}

void ASMagicProjectile::DestroyProjectile()
{
	Destroy();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

