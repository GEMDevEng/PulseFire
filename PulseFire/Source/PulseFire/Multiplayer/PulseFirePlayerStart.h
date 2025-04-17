#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PulseFirePlayerStart.generated.h"

/**
 * Player start for PulseFire.
 * Extends the standard player start with team information.
 */
UCLASS()
class PULSEFIRE_API APulseFirePlayerStart : public APlayerStart
{
    GENERATED_BODY()
    
public:
    APulseFirePlayerStart();

    /** Team index (0 = no team) */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Team")
    int32 TeamIndex;

    /** Whether this spawn point is for initial spawning only */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn")
    bool bInitialSpawnOnly;

    /** Whether this spawn point is for respawning only */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn")
    bool bRespawnOnly;

    /** Get whether this spawn point is valid for the given controller */
    bool IsValidSpawnPointFor(AController* Controller);
};
