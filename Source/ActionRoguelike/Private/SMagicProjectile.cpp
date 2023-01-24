// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	DestroyDelay = 0.2f;
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

void ASMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.f);

			if(GetWorldTimerManager().IsTimerActive(TimerHandle_DestroyProjectile))
			{
				GetWorldTimerManager().ClearTimer(TimerHandle_DestroyProjectile);
			}

			GetWorldTimerManager().SetTimer(TimerHandle_DestroyProjectile, this, &ASMagicProjectile::DestroyProjectile, DestroyDelay);
		}

		DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Blue, 1.f, 0.f);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, SweepResult.ImpactPoint, GetActorRotation());

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

