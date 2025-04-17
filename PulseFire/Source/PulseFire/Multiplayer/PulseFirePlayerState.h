#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PulseFirePlayerState.generated.h"

/**
 * Player state for PulseFire.
 * Tracks player statistics and score.
 */
UCLASS()
class PULSEFIRE_API APulseFirePlayerState : public APlayerState
{
    GENERATED_BODY()
    
public:
    APulseFirePlayerState();

    /** Initialize the player state */
    virtual void BeginPlay() override;

    /** Get the number of kills */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    int32 GetKills() const { return Kills; }

    /** Get the number of deaths */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    int32 GetDeaths() const { return Deaths; }

    /** Get the kill/death ratio */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    float GetKDRatio() const;

    /** Add a kill */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddKill();

    /** Add a death */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddDeath();

    /** Reset stats */
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ResetStats();

protected:
    /** Number of kills */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stats")
    int32 Kills;

    /** Number of deaths */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stats")
    int32 Deaths;
};
