// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include <AIModule/Classes/EnvironmentQuery/EnvQueryTypes.h>
#include <AIModule/Classes/EnvironmentQuery/EnvQueryManager.h>
#include <DrawDebugHelpers.h>
#include "AI/SAICharacter.h"
#include <Curves/CurveFloat.h>
#include "SAttributeComponent.h"
#include <EngineUtils.h>

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 5.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// a looping timer that spawns the bots
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{

	int32 NumAliveBots = 0;

	// TActorIterator lets us grab all instances of a particular class in the current world
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast< USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(AttributeComp) && AttributeComp->IsAlive()) // ensure here bc we should always have attribute comps on the AI character
		{
			NumAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	// we dont even run the query if we dont allow bot spawns
	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// queries are run asynchronously, so we bind something to when the query is finished
	if(ensure(QueryInstance)) // we need to assign this in blueprints so we ensure here that the designers set it in BP
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}

}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations;

	QueryInstance->GetQueryResultsAsLocations(Locations);
	
	// we return one random from best 5 percent, so we check for the first object to be there
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		
		UE_LOG(LogTemp, Log, TEXT("Spawning new bot"));

		// Track all the used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.0f);
	}
}

