#include "CaptureTheFlagGameMode.h"
#include "PulseFirePlayerState.h"
#include "PulseFireGameState.h"
#include "../PulseFireCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Flag.h"
#include "FlagBase.h"

ACaptureTheFlagGameMode::ACaptureTheFlagGameMode()
{
    // Set default values
    MatchTimeLimit = 600.0f; // 10 minutes
    ScoreLimit = 3;
    PointsForCapture = 1;
    PointsForReturn = 0;
    PointsForKill = 0;
    PointsForKillingCarrier = 1;
    FlagCarrierRespawnDelay = 5.0f;
    FlagReturnTime = 30.0f;
    MatchStartTime = 0.0f;

    // Initialize team scores
    TeamScores.Add(0); // Team 0 (Red)
    TeamScores.Add(0); // Team 1 (Blue)
}

void ACaptureTheFlagGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Parse options
    MatchTimeLimit = UGameplayStatics::GetIntOption(Options, TEXT("TimeLimit"), MatchTimeLimit);
    ScoreLimit = UGameplayStatics::GetIntOption(Options, TEXT("ScoreLimit"), ScoreLimit);
}

void ACaptureTheFlagGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

    // Assign player to a team
    APulseFirePlayerState* PlayerState = NewPlayerController->GetPlayerState<APulseFirePlayerState>();
    if (PlayerState)
    {
        int32 TeamIndex = AssignPlayerToTeam(NewPlayerController);
        UpdatePlayerStateTeam(PlayerState, TeamIndex);
    }
}

void ACaptureTheFlagGameMode::StartMatch()
{
    Super::StartMatch();

    // Record match start time
    MatchStartTime = GetWorld()->GetTimeSeconds();

    // Set timer for match end
    if (MatchTimeLimit > 0.0f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_MatchEnd, this, &ACaptureTheFlagGameMode::EndMatch, MatchTimeLimit, false);
    }
}

void ACaptureTheFlagGameMode::EndMatch()
{
    Super::EndMatch();

    // Determine winner
    int32 WinningTeam = GetWinningTeam();
    EndMatchWithWinner(WinningTeam);
}

void ACaptureTheFlagGameMode::OnPlayerKilled(APulseFireCharacter* KilledCharacter, AController* KillerController)
{
    if (!KilledCharacter || !KillerController)
    {
        return;
    }

    // Get player states
    APulseFirePlayerState* KilledPS = KilledCharacter->GetPlayerState<APulseFirePlayerState>();
    APulseFirePlayerState* KillerPS = KillerController->GetPlayerState<APulseFirePlayerState>();
    if (!KilledPS || !KillerPS || KilledPS == KillerPS)
    {
        return;
    }

    // Check if killed player was carrying a flag
    bool bWasCarryingFlag = false;
    // In a real implementation, we would check if the killed player was carrying a flag
    // For now, we'll just use a placeholder
    
    // Award points for kill
    if (KilledPS->GetTeamIndex() != KillerPS->GetTeamIndex())
    {
        // Award points for killing an enemy
        KillerPS->AddScore(PointsForKill);

        // Award additional points for killing the flag carrier
        if (bWasCarryingFlag)
        {
            KillerPS->AddScore(PointsForKillingCarrier);
        }
    }
}

void ACaptureTheFlagGameMode::OnFlagPickedUp(AFlag* Flag, APulseFireCharacter* Character)
{
    if (!Flag || !Character)
    {
        return;
    }

    // Get player state
    APulseFirePlayerState* PlayerState = Character->GetPlayerState<APulseFirePlayerState>();
    if (!PlayerState)
    {
        return;
    }

    // Set flag carrier
    Flag->SetCarrier(Character);

    // Broadcast flag pickup event
    // In a real implementation, we would broadcast an event to all players
}

void ACaptureTheFlagGameMode::OnFlagDropped(AFlag* Flag, APulseFireCharacter* Character)
{
    if (!Flag)
    {
        return;
    }

    // Clear flag carrier
    Flag->ClearCarrier();

    // Start flag return timer
    Flag->StartReturnTimer(FlagReturnTime);

    // Broadcast flag drop event
    // In a real implementation, we would broadcast an event to all players
}

void ACaptureTheFlagGameMode::OnFlagCaptured(AFlag* Flag, APulseFireCharacter* Character, AFlagBase* FlagBase)
{
    if (!Flag || !Character || !FlagBase)
    {
        return;
    }

    // Get player state
    APulseFirePlayerState* PlayerState = Character->GetPlayerState<APulseFirePlayerState>();
    if (!PlayerState)
    {
        return;
    }

    // Award points for capture
    int32 TeamIndex = PlayerState->GetTeamIndex();
    AddTeamScore(TeamIndex, PointsForCapture);
    PlayerState->AddScore(PointsForCapture);

    // Return flag to base
    Flag->ReturnToBase();

    // Broadcast flag capture event
    // In a real implementation, we would broadcast an event to all players

    // Check if the match should end
    CheckMatchEnd();
}

void ACaptureTheFlagGameMode::OnFlagReturned(AFlag* Flag)
{
    if (!Flag)
    {
        return;
    }

    // Return flag to base
    Flag->ReturnToBase();

    // Broadcast flag return event
    // In a real implementation, we would broadcast an event to all players
}

int32 ACaptureTheFlagGameMode::GetTeamScore(int32 TeamIndex) const
{
    if (TeamIndex >= 0 && TeamIndex < TeamScores.Num())
    {
        return TeamScores[TeamIndex];
    }
    return 0;
}

void ACaptureTheFlagGameMode::SetTeamScore(int32 TeamIndex, int32 NewScore)
{
    if (TeamIndex >= 0 && TeamIndex < TeamScores.Num())
    {
        TeamScores[TeamIndex] = NewScore;
    }
}

void ACaptureTheFlagGameMode::AddTeamScore(int32 TeamIndex, int32 ScoreToAdd)
{
    if (TeamIndex >= 0 && TeamIndex < TeamScores.Num())
    {
        TeamScores[TeamIndex] += ScoreToAdd;
    }
}

float ACaptureTheFlagGameMode::GetMatchTimeRemaining() const
{
    if (MatchTimeLimit <= 0.0f)
    {
        return 0.0f;
    }

    float ElapsedTime = GetMatchTimeElapsed();
    return FMath::Max(0.0f, MatchTimeLimit - ElapsedTime);
}

float ACaptureTheFlagGameMode::GetMatchTimeElapsed() const
{
    if (MatchStartTime <= 0.0f)
    {
        return 0.0f;
    }

    return GetWorld()->GetTimeSeconds() - MatchStartTime;
}

float ACaptureTheFlagGameMode::GetMatchTimeLimit() const
{
    return MatchTimeLimit;
}

int32 ACaptureTheFlagGameMode::GetScoreLimit() const
{
    return ScoreLimit;
}

void ACaptureTheFlagGameMode::CheckMatchEnd()
{
    // Check if any team has reached the score limit
    for (int32 TeamIndex = 0; TeamIndex < TeamScores.Num(); TeamIndex++)
    {
        if (TeamScores[TeamIndex] >= ScoreLimit)
        {
            EndMatchWithWinner(TeamIndex);
            break;
        }
    }
}

void ACaptureTheFlagGameMode::EndMatchWithWinner(int32 WinningTeam)
{
    // End the match
    EndMatch();

    // Broadcast match end event with winner
    // In a real implementation, we would broadcast an event to all players
}

int32 ACaptureTheFlagGameMode::GetWinningTeam() const
{
    int32 WinningTeam = -1;
    int32 HighestScore = -1;

    // Find team with highest score
    for (int32 TeamIndex = 0; TeamIndex < TeamScores.Num(); TeamIndex++)
    {
        if (TeamScores[TeamIndex] > HighestScore)
        {
            HighestScore = TeamScores[TeamIndex];
            WinningTeam = TeamIndex;
        }
    }

    return WinningTeam;
}

void ACaptureTheFlagGameMode::UpdatePlayerStateTeam(APulseFirePlayerState* PlayerState, int32 TeamIndex)
{
    if (PlayerState)
    {
        PlayerState->SetTeamIndex(TeamIndex);
    }
}

int32 ACaptureTheFlagGameMode::AssignPlayerToTeam(APlayerController* PlayerController)
{
    // Count players on each team
    TArray<int32> PlayersPerTeam;
    CountPlayersPerTeam(PlayersPerTeam);

    // Assign to team with fewer players
    return GetTeamWithFewerPlayers();
}

int32 ACaptureTheFlagGameMode::GetTeamWithFewerPlayers() const
{
    // Count players on each team
    TArray<int32> PlayersPerTeam;
    CountPlayersPerTeam(PlayersPerTeam);

    // Find team with fewer players
    int32 TeamWithFewerPlayers = 0;
    for (int32 TeamIndex = 1; TeamIndex < PlayersPerTeam.Num(); TeamIndex++)
    {
        if (PlayersPerTeam[TeamIndex] < PlayersPerTeam[TeamWithFewerPlayers])
        {
            TeamWithFewerPlayers = TeamIndex;
        }
    }

    return TeamWithFewerPlayers;
}

void ACaptureTheFlagGameMode::CountPlayersPerTeam(TArray<int32>& PlayersPerTeam) const
{
    // Initialize array
    PlayersPerTeam.Empty();
    PlayersPerTeam.Add(0); // Team 0 (Red)
    PlayersPerTeam.Add(0); // Team 1 (Blue)

    // Count players on each team
    for (TActorIterator<APlayerController> It(GetWorld()); It; ++It)
    {
        APlayerController* PC = *It;
        if (PC)
        {
            APulseFirePlayerState* PS = PC->GetPlayerState<APulseFirePlayerState>();
            if (PS)
            {
                int32 TeamIndex = PS->GetTeamIndex();
                if (TeamIndex >= 0 && TeamIndex < PlayersPerTeam.Num())
                {
                    PlayersPerTeam[TeamIndex]++;
                }
            }
        }
    }
}
