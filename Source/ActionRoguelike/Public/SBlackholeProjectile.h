// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SBlackholeProjectile.generated.h"

class URadialForceComponent;
class USphereComponent;
class UPrimitiveComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASBlackholeProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
		ASBlackholeProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* DestroySphereComp;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
