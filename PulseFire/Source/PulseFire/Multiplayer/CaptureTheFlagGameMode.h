#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

class AFlagBase;
class AFlag;
class APulseFireCharacter;
class APulseFirePlayerState;

/**
 * Game mode for Capture the Flag gameplay.
 */
UCLASS()
class PULSEFIRE_API ACaptureTheFlagGameMode : public AGameMode
{
    GENERATED_BODY()
    
public:
    ACaptureTheFlagGameMode();

    /** Initialize the game mode */
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    /** Initialize a newly spawned player */
    virtual void InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

    /** Start the match */
    virtual void StartMatch() override;

    /** End the match */
    virtual void EndMatch() override;

    /** Handle player death */
    virtual void OnPlayerKilled(APulseFireCharacter* KilledCharacter, AController* KillerController);

    /** Handle flag pickup */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    virtual void OnFlagPickedUp(AFlag* Flag, APulseFireCharacter* Character);

    /** Handle flag drop */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    virtual void OnFlagDropped(AFlag* Flag, APulseFireCharacter* Character);

    /** Handle flag capture */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    virtual void OnFlagCaptured(AFlag* Flag, APulseFireCharacter* Character, AFlagBase* FlagBase);

    /** Handle flag return */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    virtual void OnFlagReturned(AFlag* Flag);

    /** Get team score */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    int32 GetTeamScore(int32 TeamIndex) const;

    /** Set team score */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    void SetTeamScore(int32 TeamIndex, int32 NewScore);

    /** Add to team score */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    void AddTeamScore(int32 TeamIndex, int32 ScoreToAdd);

    /** Get match time remaining */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    float GetMatchTimeRemaining() const;

    /** Get match time elapsed */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    float GetMatchTimeElapsed() const;

    /** Get match time limit */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    float GetMatchTimeLimit() const;

    /** Get score limit */
    UFUNCTION(BlueprintCallable, Category = "Capture The Flag")
    int32 GetScoreLimit() const;

protected:
    /** Match time limit in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    float MatchTimeLimit;

    /** Score limit */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    int32 ScoreLimit;

    /** Points for capturing a flag */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    int32 PointsForCapture;

    /** Points for returning a flag */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    int32 PointsForReturn;

    /** Points for killing an enemy */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    int32 PointsForKill;

    /** Points for killing the flag carrier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    int32 PointsForKillingCarrier;

    /** Respawn delay for flag carrier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    float FlagCarrierRespawnDelay;

    /** Flag return time (seconds) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Capture The Flag")
    float FlagReturnTime;

    /** Team scores */
    UPROPERTY(BlueprintReadOnly, Category = "Capture The Flag")
    TArray<int32> TeamScores;

    /** Match start time */
    float MatchStartTime;

    /** Timer handle for match end */
    FTimerHandle TimerHandle_MatchEnd;

    /** Check if the match should end */
    void CheckMatchEnd();

    /** End the match with the specified winner */
    void EndMatchWithWinner(int32 WinningTeam);

    /** Get the winning team */
    int32 GetWinningTeam() const;

    /** Update player state with team information */
    void UpdatePlayerStateTeam(APulseFirePlayerState* PlayerState, int32 TeamIndex);

    /** Assign player to a team */
    int32 AssignPlayerToTeam(APlayerController* PlayerController);

    /** Get team with fewer players */
    int32 GetTeamWithFewerPlayers() const;

    /** Count players on each team */
    void CountPlayersPerTeam(TArray<int32>& PlayersPerTeam) const;
};
