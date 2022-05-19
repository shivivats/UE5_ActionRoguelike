// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "DrawDebugHelpers.h"
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

	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnHit);
}

void ASMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor!=GetInstigator())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 50.f, 12, FColor::Blue, 1.f, 0.f);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, GetActorRotation());

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

