// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SAttributeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMagicProjectile.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SProjectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// makes the character able to rotate independent of the camera rotation
	bUseControllerRotationYaw = false;

	PrimaryAttackSpawnDelay = 0.2f;
	BlackHoleAttackSpawnDelay = 0.2f;
	TeleportAttackSpawnDelay = 0.2f;

	TimeToHitParamName = "TimeToHit";
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	FVector ControlRightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);


	AddMovementInput(ControlRightVector * Value);
}

FRotator ASCharacter::GetProjectileRotation(FVector SpawnLocation)
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
	CollisionQueryParams.AddIgnoredActor(this);

	FVector EyeLocation;
	FRotator EyeRotation;
	this->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = CameraComp->GetComponentLocation();

	// Maybe the multiplier should be made into a variable but I think its okay like this
	FVector End = Start + (CameraComp->GetForwardVector() * 10000.f);

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams, CollisionQueryParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.f);

	FRotator ProjectileRotation;

	if (bBlockingHit)
	{
		ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Hit.ImpactPoint);

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32, FColor::Red, false, 2.0f);
	}
	else
	{
		ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, End);
	}

	return ProjectileRotation;
}

AActor* ASCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass, FVector SpawnLocation, UParticleSystem* SpawnEffect)
{
	// First we get the rotation of the projectile from our own function
	FRotator ProjectileRotation = GetProjectileRotation(SpawnLocation);

	// before spawning a projectile we play the particle effect for casting it
	UGameplayStatics::SpawnEmitterAttached(SpawnEffect, GetMesh(), FName("Muzzle_01"), GetMesh()->GetSocketLocation("Muzzle_01"), FRotator(), EAttachLocation::KeepWorldPosition);

	// Here we actually spawn the projectile with the rotation we derived
	FTransform SpawnTransform = FTransform(ProjectileRotation, SpawnLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);

	return SpawnedProjectile;
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(PrimaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, PrimaryAttackSpawnDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensure(PrimaryProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		SpawnProjectile(PrimaryProjectileClass, HandLocation, PrimaryProjectileSpawnEffect);
	}
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(BlackHoleAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, BlackHoleAttackSpawnDelay);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	if (ensure(BlackHoleProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		SpawnProjectile(BlackHoleProjectileClass, HandLocation, BlackHoleSpawnEffect);
	}
}

void ASCharacter::TeleportAttack()
{
	PlayAnimMontage(TeleportAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAttack, this, &ASCharacter::TeleportAttack_TimeElapsed, TeleportAttackSpawnDelay);
}

void ASCharacter::TeleportAttack_TimeElapsed()
{
	if (ensure(TeleportAttackProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		SpawnProjectile(TeleportAttackProjectileClass, HandLocation, TeleportAttackSpawnEffect);
	}
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		// if damaged
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);


		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}

	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &ASCharacter::TeleportAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

