#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PulseFireGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, FName, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerKilled, class APulseFireCharacter*, KilledPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerScoreChanged, class APlayerState*, PlayerState, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEnded);

/**
 * Game mode for PulseFire multiplayer matches.
 */
UCLASS()
class PULSEFIRE_API APulseFireGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    APulseFireGameMode();

    /** Initialize the game mode */
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    /** Called when the game starts */
    virtual void BeginPlay() override;

    /** Called every frame */
    virtual void Tick(float DeltaSeconds) override;

    /** Called when a player joins the game */
    virtual void PostLogin(APlayerController* NewPlayer) override;

    /** Called when a player leaves the game */
    virtual void Logout(AController* Exiting) override;

    /** Start the match */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartMatch();

    /** End the match */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void EndMatch();

    /** Restart the match */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void RestartMatch();

    /** Get the current match state */
    UFUNCTION(BlueprintCallable, Category = "Game")
    FName GetMatchState() const { return MatchState; }

    /** Get the match time remaining */
    UFUNCTION(BlueprintCallable, Category = "Game")
    float GetMatchTimeRemaining() const { return MatchTimeRemaining; }

    /** Get the match duration */
    UFUNCTION(BlueprintCallable, Category = "Game")
    float GetMatchDuration() const { return MatchDuration; }

    /** Get whether the match is in progress */
    UFUNCTION(BlueprintCallable, Category = "Game")
    bool IsMatchInProgress() const { return MatchState == FName("InProgress"); }

    /** Handle player killed event */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void OnPlayerKilled(class APulseFireCharacter* KilledPlayer, class AController* KillerController);

    /** Respawn a player */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void RespawnPlayer(class AController* Controller);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnMatchStateChanged OnMatchStateChangedEvent;

    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnPlayerKilled OnPlayerKilledEvent;

    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnPlayerScoreChanged OnPlayerScoreChangedEvent;

    UPROPERTY(BlueprintAssignable, Category = "Game")
    FOnMatchEnded OnMatchEndedEvent;

protected:
    /** Match duration in seconds (default: 5 minutes) */
    UPROPERTY(EditDefaultsOnly, Category = "Match Settings")
    float MatchDuration;

    /** Current match time remaining in seconds */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match State")
    float MatchTimeRemaining;

    /** Current match state */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match State")
    FName MatchState;

    /** Respawn delay in seconds */
    UPROPERTY(EditDefaultsOnly, Category = "Match Settings")
    float RespawnDelay;

    /** Maximum number of players */
    UPROPERTY(EditDefaultsOnly, Category = "Match Settings")
    int32 MaxPlayers;

    /** Score limit (0 = no limit) */
    UPROPERTY(EditDefaultsOnly, Category = "Match Settings")
    int32 ScoreLimit;

    /** Called when the match time expires */
    UFUNCTION(BlueprintNativeEvent, Category = "Match Events")
    void OnMatchTimeExpired();
    virtual void OnMatchTimeExpired_Implementation();

    /** Set the match state */
    UFUNCTION(BlueprintCallable, Category = "Game")
    void SetMatchState(FName NewState);

    /** Get a random spawn point */
    UFUNCTION(BlueprintCallable, Category = "Game")
    class AActor* GetRandomSpawnPoint();

    /** Timer handle for respawning players */
    TMap<AController*, FTimerHandle> RespawnTimers;
};
