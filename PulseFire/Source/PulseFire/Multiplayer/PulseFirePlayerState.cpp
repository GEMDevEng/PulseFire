#include "PulseFirePlayerState.h"
#include "Net/UnrealNetwork.h"

APulseFirePlayerState::APulseFirePlayerState()
{
    // Set default values
    Kills = 0;
    Deaths = 0;
    TeamIndex = 0;
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
    DOREPLIFETIME(APulseFirePlayerState, TeamIndex);
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

void APulseFirePlayerState::SetTeamIndex(int32 NewTeamIndex)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        TeamIndex = NewTeamIndex;
    }
}

FLinearColor APulseFirePlayerState::GetTeamColor() const
{
    // Return color based on team index
    switch (TeamIndex)
    {
    case 1:
        return FLinearColor(0.0f, 0.0f, 1.0f); // Blue
    case 2:
        return FLinearColor(1.0f, 0.0f, 0.0f); // Red
    case 3:
        return FLinearColor(0.0f, 1.0f, 0.0f); // Green
    case 4:
        return FLinearColor(1.0f, 1.0f, 0.0f); // Yellow
    default:
        return FLinearColor(0.5f, 0.5f, 0.5f); // Gray (no team)
    }
}
