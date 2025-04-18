// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PulseFireGameMode.generated.h"

/**
 * The game mode for PulseFire multiplayer matches.
 * Handles match timing, scoring, and player spawning.
 */
UCLASS(minimalapi)
class APulseFireGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APulseFireGameMode();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaSeconds) override;

protected:
	/** Match duration in seconds (default: 5 minutes) */
	UPROPERTY(EditDefaultsOnly, Category = "Match Settings")
	float MatchDuration;

	/** Current match time remaining in seconds */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match State")
	float MatchTimeRemaining;

	/** Whether the match is currently in progress */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match State")
	bool bMatchInProgress;

	/** Called when the match time expires */
	UFUNCTION(BlueprintNativeEvent, Category = "Match Events")
	void OnMatchTimeExpired();
	virtual void OnMatchTimeExpired_Implementation();

	/** Starts the match timer */
	UFUNCTION(BlueprintCallable, Category = "Match Control")
	void StartMatch();

	/** Ends the match and shows results */
	UFUNCTION(BlueprintCallable, Category = "Match Control")
	void EndMatch();
};
