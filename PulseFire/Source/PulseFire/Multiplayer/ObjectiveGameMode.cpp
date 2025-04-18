#include "ObjectiveGameMode.h"
#include "PulseFirePlayerState.h"
#include "PulseFirePlayerStart.h"
#include "../Gameplay/CapturePoint.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

AObjectiveGameMode::AObjectiveGameMode()
{
    // Set default values
    NumTeams = 2;
    ScoreToWin = 100;
    ScorePerCapturePointPerSecond = 1;
}

void AObjectiveGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Parse options
    if (UGameplayStatics::HasOption(Options, TEXT("NumTeams")))
    {
        NumTeams = FCString::Atoi(*UGameplayStatics::ParseOption(Options, TEXT("NumTeams")));
    }

    if (UGameplayStatics::HasOption(Options, TEXT("ScoreToWin")))
    {
        ScoreToWin = FCString::Atoi(*UGameplayStatics::ParseOption(Options, TEXT("ScoreToWin")));
    }

    // Initialize team scores
    TeamScores.SetNum(NumTeams + 1); // +1 because team indices start at 1
    for (int32 i = 0; i <= NumTeams; i++)
    {
        TeamScores[i] = 0;
    }
}

void AObjectiveGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find all capture points in the level
    FindCapturePoints();

    // Start score update timer
    GetWorldTimerManager().SetTimer(TimerHandle_ScoreUpdate, this, &AObjectiveGameMode::UpdateScores, 1.0f, true);
}

void AObjectiveGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AObjectiveGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate team scores to all clients
    DOREPLIFETIME(AObjectiveGameMode, TeamScores);
}

void AObjectiveGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Assign player to a team
    AssignPlayerToTeam(NewPlayer);
}

int32 AObjectiveGameMode::GetTeamScore(int32 TeamIndex) const
{
    if (TeamIndex >= 0 && TeamIndex < TeamScores.Num())
    {
        return TeamScores[TeamIndex];
    }
    return 0;
}

void AObjectiveGameMode::AddTeamScore(int32 TeamIndex, int32 ScoreToAdd)
{
    if (TeamIndex > 0 && TeamIndex <= NumTeams)
    {
        // Add score
        TeamScores[TeamIndex] += ScoreToAdd;

        // Broadcast score change
        OnTeamScoreChanged.Broadcast(TeamIndex);

        // Check for win condition
        if (TeamScores[TeamIndex] >= ScoreToWin)
        {
            // End the match
            EndMatch();
        }
    }
}

int32 AObjectiveGameMode::GetWinningTeam() const
{
    int32 WinningTeam = 0;
    int32 HighestScore = -1;
    bool bTied = false;

    // Find team with highest score
    for (int32 i = 1; i <= NumTeams; i++)
    {
        if (TeamScores[i] > HighestScore)
        {
            HighestScore = TeamScores[i];
            WinningTeam = i;
            bTied = false;
        }
        else if (TeamScores[i] == HighestScore)
        {
            bTied = true;
        }
    }

    // If tied, no winner
    if (bTied)
    {
        return 0;
    }

    return WinningTeam;
}

void AObjectiveGameMode::AssignPlayerToTeam(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        return;
    }

    // Get player state
    APulseFirePlayerState* PlayerState = PlayerController->GetPlayerState<APulseFirePlayerState>();
    if (!PlayerState)
    {
        return;
    }

    // Count players on each team
    TArray<int32> TeamCounts;
    TeamCounts.SetNum(NumTeams + 1);
    for (int32 i = 0; i <= NumTeams; i++)
    {
        TeamCounts[i] = 0;
    }

    // Count players on each team
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC != PlayerController)
        {
            APulseFirePlayerState* PS = PC->GetPlayerState<APulseFirePlayerState>();
            if (PS)
            {
                int32 TeamIndex = PS->GetTeamIndex();
                if (TeamIndex > 0 && TeamIndex <= NumTeams)
                {
                    TeamCounts[TeamIndex]++;
                }
            }
        }
    }

    // Find team with fewest players
    int32 TeamIndex = 1;
    int32 MinCount = TeamCounts[1];
    for (int32 i = 2; i <= NumTeams; i++)
    {
        if (TeamCounts[i] < MinCount)
        {
            MinCount = TeamCounts[i];
            TeamIndex = i;
        }
    }

    // Assign player to team
    PlayerState->SetTeamIndex(TeamIndex);
}

void AObjectiveGameMode::UpdateScores()
{
    // Update scores based on capture points
    for (ACapturePoint* CapturePoint : CapturePoints)
    {
        if (CapturePoint)
        {
            int32 OwningTeam = CapturePoint->GetOwningTeam();
            if (OwningTeam > 0)
            {
                // Add score for owning the capture point
                AddTeamScore(OwningTeam, ScorePerCapturePointPerSecond);
            }
        }
    }
}

void AObjectiveGameMode::OnCapturePointCaptured(int32 TeamIndex)
{
    // Add bonus score for capturing a point
    if (TeamIndex > 0)
    {
        AddTeamScore(TeamIndex, 10);
    }
}

void AObjectiveGameMode::OnMatchTimeExpired_Implementation()
{
    // End the match when time expires
    EndMatch();
}

void AObjectiveGameMode::FindCapturePoints()
{
    // Find all capture points in the level
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturePoint::StaticClass(), FoundActors);

    // Add to capture points array
    for (AActor* Actor : FoundActors)
    {
        ACapturePoint* CapturePoint = Cast<ACapturePoint>(Actor);
        if (CapturePoint)
        {
            CapturePoints.Add(CapturePoint);

            // Register for capture events
            CapturePoint->OnCapturePointCaptured.AddDynamic(this, &AObjectiveGameMode::OnCapturePointCaptured);
        }
    }
}
