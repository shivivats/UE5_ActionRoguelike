// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ASMagicProjectile;
class USInteractionComponent;
class UAnimMontage;
class ASProjectile;
class USAttributeComponent;
class UCameraShakeBase;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParameterName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	/* Primary Left Click Attack */
	UPROPERTY(EditAnywhere, Category = "Attack|Primary Attack")
	TSubclassOf<ASMagicProjectile> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Primary Attack")
	UAnimMontage* PrimaryAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack|Primary Attack")
	UParticleSystem* PrimaryProjectileSpawnEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float PrimaryAttackSpawnDelay;

	FTimerHandle TimerHandle_PrimaryAttack;

	/* Black Hole Attack */
	UPROPERTY(EditAnywhere, Category = "Attack|Black Hole Attack")
	TSubclassOf<ASProjectile> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Black Hole Attack")
	UAnimMontage* BlackHoleAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack|Black Hole Attack")
	UParticleSystem* BlackHoleSpawnEffect;
	
	UPROPERTY(EditAnywhere, Category = "Attack|Black Hole Attack")
	float BlackHoleAttackSpawnDelay;

	FTimerHandle TimerHandle_BlackHoleAttack;

	/* Teleportation Attack */
	UPROPERTY(EditAnywhere, Category = "Attack|Teleport Attack")
	TSubclassOf<ASProjectile> TeleportAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack|Teleport Attack")
	UAnimMontage* TeleportAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack|Teleport Attack")
	UParticleSystem* TeleportAttackSpawnEffect;
	
	UPROPERTY(EditAnywhere, Category = "Attack|Teleport Attack")
	float TeleportAttackSpawnDelay;

	FTimerHandle TimerHandle_TeleportAttack;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	FRotator GetProjectileRotation(FVector SpawnLocation);

	AActor* SpawnProjectile(TSubclassOf<AActor> ProjectileClass, FVector SpawnLocation, UParticleSystem* SpawnEffect);

	/* Primary Left Click Attack */
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	/* Black Hole Attack */
	void BlackHoleAttack();

	void BlackHoleAttack_TimeElapsed();

	/* Teleportation Attack */
	void TeleportAttack();

	void TeleportAttack_TimeElapsed();

	/* Interaction */
	void PrimaryInteract();

	/* Health Stuff */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
