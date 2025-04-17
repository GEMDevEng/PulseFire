#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PulseFireGameState.generated.h"

/**
 * Game state for PulseFire.
 * Tracks match state and time.
 */
UCLASS()
class PULSEFIRE_API APulseFireGameState : public AGameStateBase
{
    GENERATED_BODY()
    
public:
    APulseFireGameState();

    /** Initialize the game state */
    virtual void BeginPlay() override;

    /** Called every frame */
    virtual void Tick(float DeltaSeconds) override;

    /** Get the match time remaining */
    UFUNCTION(BlueprintCallable, Category = "Match")
    float GetMatchTimeRemaining() const { return MatchTimeRemaining; }

    /** Get the match duration */
    UFUNCTION(BlueprintCallable, Category = "Match")
    float GetMatchDuration() const { return MatchDuration; }

    /** Get the match state */
    UFUNCTION(BlueprintCallable, Category = "Match")
    FName GetMatchState() const { return MatchState; }

    /** Get whether the match is in progress */
    UFUNCTION(BlueprintCallable, Category = "Match")
    bool IsMatchInProgress() const { return MatchState == FName("InProgress"); }

    /** Set the match time remaining */
    UFUNCTION(BlueprintCallable, Category = "Match")
    void SetMatchTimeRemaining(float NewTime);

    /** Set the match duration */
    UFUNCTION(BlueprintCallable, Category = "Match")
    void SetMatchDuration(float NewDuration);

    /** Set the match state */
    UFUNCTION(BlueprintCallable, Category = "Match")
    void SetMatchState(FName NewState);

protected:
    /** Match duration in seconds */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match")
    float MatchDuration;

    /** Match time remaining in seconds */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match")
    float MatchTimeRemaining;

    /** Current match state */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match")
    FName MatchState;
};
