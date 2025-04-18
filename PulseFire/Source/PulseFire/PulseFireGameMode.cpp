// Copyright Epic Games, Inc. All Rights Reserved.

#include "PulseFireGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

APulseFireGameMode::APulseFireGameMode()
{
	// Set default values
	MatchDuration = 300.0f; // 5 minutes
	MatchTimeRemaining = MatchDuration;
	bMatchInProgress = false;

	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;
}

void APulseFireGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Additional initialization can be done here
}

void APulseFireGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update match timer if match is in progress
	if (bMatchInProgress)
	{
		MatchTimeRemaining -= DeltaSeconds;
		
		// Check if match time has expired
		if (MatchTimeRemaining <= 0.0f)
		{
			MatchTimeRemaining = 0.0f;
			OnMatchTimeExpired();
		}
	}
}

void APulseFireGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate match state to all clients
	DOREPLIFETIME(APulseFireGameMode, MatchTimeRemaining);
	DOREPLIFETIME(APulseFireGameMode, bMatchInProgress);
}

void APulseFireGameMode::StartMatch()
{
	if (!bMatchInProgress)
	{
		MatchTimeRemaining = MatchDuration;
		bMatchInProgress = true;
		
		// Additional match start logic can be added here
	}
}

void APulseFireGameMode::EndMatch()
{
	if (bMatchInProgress)
	{
		bMatchInProgress = false;
		
		// Additional match end logic can be added here
	}
}

void APulseFireGameMode::OnMatchTimeExpired_Implementation()
{
	// End the match when time expires
	EndMatch();
}
