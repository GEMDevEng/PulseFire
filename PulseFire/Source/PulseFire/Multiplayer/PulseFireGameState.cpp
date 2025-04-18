#include "PulseFireGameState.h"
#include "Net/UnrealNetwork.h"

APulseFireGameState::APulseFireGameState()
{
    // Set default values
    MatchDuration = 300.0f; // 5 minutes
    MatchTimeRemaining = MatchDuration;
    MatchState = FName("WaitingToStart");

    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
}

void APulseFireGameState::BeginPlay()
{
    Super::BeginPlay();
}

void APulseFireGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void APulseFireGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate match state to all clients
    DOREPLIFETIME(APulseFireGameState, MatchDuration);
    DOREPLIFETIME(APulseFireGameState, MatchTimeRemaining);
    DOREPLIFETIME(APulseFireGameState, MatchState);
}

void APulseFireGameState::SetMatchTimeRemaining(float NewTime)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        MatchTimeRemaining = FMath::Max(0.0f, NewTime);
    }
}

void APulseFireGameState::SetMatchDuration(float NewDuration)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        MatchDuration = FMath::Max(0.0f, NewDuration);
    }
}

void APulseFireGameState::SetMatchState(FName NewState)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        MatchState = NewState;
    }
}
