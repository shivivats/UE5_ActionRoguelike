// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	UParticleSystem* DetonationEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float DetonationDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float TeleportationDelay;

	FTimerHandle TimerHandle_DetonationTimer;

	FTimerHandle TimerHandle_TeleportationTimer;

	void Detonate();

	void TeleportInstigator();
};
