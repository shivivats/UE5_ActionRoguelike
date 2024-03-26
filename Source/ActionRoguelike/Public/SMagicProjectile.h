// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectile.h"
#include "SMagicProjectile.generated.h"

class UCameraShakeBase;
class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float DestroyDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass;


	FTimerHandle TimerHandle_DestroyProjectile;

	void DestroyProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Shake")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};