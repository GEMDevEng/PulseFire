#include "PulseFirePlayerStart.h"
#include "GameFramework/Controller.h"
#include "PulseFirePlayerState.h"

APulseFirePlayerStart::APulseFirePlayerStart()
{
    // Set default values
    TeamIndex = 0;
    bInitialSpawnOnly = false;
    bRespawnOnly = false;
}

bool APulseFirePlayerStart::IsValidSpawnPointFor(AController* Controller)
{
    // If no controller, this spawn point is valid
    if (!Controller)
    {
        return true;
    }

    // Check team index
    if (TeamIndex > 0)
    {
        // Get player state
        APulseFirePlayerState* PlayerState = Controller->GetPlayerState<APulseFirePlayerState>();
        if (PlayerState && PlayerState->GetTeamIndex() != TeamIndex)
        {
            // Team doesn't match
            return false;
        }
    }

    // Check spawn type
    if (bInitialSpawnOnly)
    {
        // Check if the controller has a pawn
        if (Controller->GetPawn())
        {
            // Controller already has a pawn, so this is not an initial spawn
            return false;
        }
    }
    else if (bRespawnOnly)
    {
        // Check if the controller has a pawn
        if (!Controller->GetPawn())
        {
            // Controller doesn't have a pawn, so this is an initial spawn
            return false;
        }
    }

    return true;
}
