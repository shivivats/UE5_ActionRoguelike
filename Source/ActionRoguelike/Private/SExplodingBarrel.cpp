// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplodingBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASExplodingBarrel::ASExplodingBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent =  StaticMeshComponent;
	StaticMeshComponent->SetSimulatePhysics(true);


	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);

	RadialForceComponent->Radius = 1000.f;
	RadialForceComponent->ImpulseStrength = 200000.f;

	// Auto activate makes it so that the RadialForceComponent applies a little bit of force during tick
	// Disabling or enabling it, both are perfectly fine here
	RadialForceComponent->SetAutoActivate(false);

	// Default constructor of RadialForceComponent doesn't add WorldDynamic, so we add it here
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplodingBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// This function runs after the constructor but before BeginPlay
	// So we know this binding will happen in time before BeginPlay happens

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplodingBarrel::OnStaticMeshHit);
}

void ASExplodingBarrel::OnStaticMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in ExplodingBarrel"));

	// Use GetNameSafe here as it is safe in case OtherActor is NULL
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());

	// We can use DrawDebugString to draw a string in the world in 3D, very useful for debugging
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.f, true);

	//if(OtherActor)
	//{
	//	OtherActor->Destroy();
	//}
}

// Called every frame
void ASExplodingBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

