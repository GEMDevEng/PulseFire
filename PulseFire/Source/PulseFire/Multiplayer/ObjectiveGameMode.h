#pragma once

#include "CoreMinimal.h"
#include "PulseFireGameMode.h"
#include "ObjectiveGameMode.generated.h"

class ACapturePoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamScoreChanged, int32, TeamIndex);

/**
 * Game mode for objective-based gameplay.
 */
UCLASS()
class PULSEFIRE_API AObjectiveGameMode : public APulseFireGameMode
{
    GENERATED_BODY()
    
public:
    AObjectiveGameMode();

    /** Initialize the game mode */
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    /** Called when the game starts */
    virtual void BeginPlay() override;

    /** Called every frame */
    virtual void Tick(float DeltaSeconds) override;

    /** Called when a player joins the game */
    virtual void PostLogin(APlayerController* NewPlayer) override;

    /** Get the score for a team */
    UFUNCTION(BlueprintCallable, Category = "Objective")
    int32 GetTeamScore(int32 TeamIndex) const;

    /** Add score to a team */
    UFUNCTION(BlueprintCallable, Category = "Objective")
    void AddTeamScore(int32 TeamIndex, int32 ScoreToAdd);

    /** Get the number of teams */
    UFUNCTION(BlueprintCallable, Category = "Objective")
    int32 GetNumTeams() const { return NumTeams; }

    /** Get the team with the highest score */
    UFUNCTION(BlueprintCallable, Category = "Objective")
    int32 GetWinningTeam() const;

    /** Assign a player to a team */
    UFUNCTION(BlueprintCallable, Category = "Objective")
    void AssignPlayerToTeam(APlayerController* PlayerController);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Objective")
    FOnTeamScoreChanged OnTeamScoreChanged;

protected:
    /** Number of teams */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
    int32 NumTeams;

    /** Score to win */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
    int32 ScoreToWin;

    /** Score per capture point per second */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
    int32 ScorePerCapturePointPerSecond;

    /** Team scores */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Objective")
    TArray<int32> TeamScores;

    /** Capture points in the level */
    UPROPERTY(BlueprintReadOnly, Category = "Objective")
    TArray<ACapturePoint*> CapturePoints;

    /** Timer handle for score update */
    FTimerHandle TimerHandle_ScoreUpdate;

    /** Update scores based on capture points */
    UFUNCTION()
    void UpdateScores();

    /** Called when a capture point is captured */
    UFUNCTION()
    void OnCapturePointCaptured(int32 TeamIndex);

    /** Called when the match time expires */
    virtual void OnMatchTimeExpired_Implementation() override;

    /** Find all capture points in the level */
    void FindCapturePoints();
};
