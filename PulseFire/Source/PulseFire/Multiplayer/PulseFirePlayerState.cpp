#include "PulseFirePlayerState.h"
#include "Net/UnrealNetwork.h"

APulseFirePlayerState::APulseFirePlayerState()
{
    // Set default values
    Kills = 0;
    Deaths = 0;
}

void APulseFirePlayerState::BeginPlay()
{
    Super::BeginPlay();
}

void APulseFirePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate stats to all clients
    DOREPLIFETIME(APulseFirePlayerState, Kills);
    DOREPLIFETIME(APulseFirePlayerState, Deaths);
}

float APulseFirePlayerState::GetKDRatio() const
{
    if (Deaths == 0)
    {
        return static_cast<float>(Kills);
    }
    
    return static_cast<float>(Kills) / static_cast<float>(Deaths);
}

void APulseFirePlayerState::AddKill()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Kills++;
        
        // Update score
        SetScore(GetScore() + 1);
    }
}

void APulseFirePlayerState::AddDeath()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Deaths++;
    }
}

void APulseFirePlayerState::ResetStats()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Kills = 0;
        Deaths = 0;
        SetScore(0);
    }
}
