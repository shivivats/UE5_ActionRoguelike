// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "SProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	ProjectileAttackSpawnDelay = 0.2f;
	HandSocketName = FName("Muzzle_01");
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASCharacter* Character = Cast<ASCharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(ProjectileAttackAnim);

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "ProjectileAttack_TimeElapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ProjectileAttack, TimerDelegate, ProjectileAttackSpawnDelay, false);
	}

}

void USAction_ProjectileAttack::ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		// Here we do a LineTrace to get where the projectile should hit
		// So we can use the result to calculate its rotation for later
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);

		FVector EyeLocation;
		FRotator EyeRotation;
		InstigatorCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		// Maybe the multiplier should be made into a variable but I think its okay like this for now
		// multiplier signifies how far we look into the distance for the trace
		FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 5000.f;

		FHitResult Hit;
		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams, CollisionQueryParams);

		FRotator ProjectileRotation;

		FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		if (bBlockingHit)
		{
			ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Hit.ImpactPoint);

			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32, FColor::Red, false, 2.0f);
		}
		else
		{
			ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TraceEnd);
		}

		// before spawning a projectile we play the particle effect for casting it
		UGameplayStatics::SpawnEmitterAttached(ProjectileSpawnEffect, InstigatorCharacter->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		// Here we actually spawn the projectile with the rotation we derived
		FTransform SpawnTransform = FTransform(ProjectileRotation, SpawnLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
