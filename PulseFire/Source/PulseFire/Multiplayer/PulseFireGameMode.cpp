#include "PulseFireGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "../PulseFireCharacter.h"
#include "PulseFirePlayerState.h"

APulseFireGameMode::APulseFireGameMode()
{
    // Set default values
    MatchDuration = 300.0f; // 5 minutes
    MatchTimeRemaining = MatchDuration;
    MatchState = FName("WaitingToStart");
    RespawnDelay = 5.0f;
    MaxPlayers = 8;
    ScoreLimit = 0; // No score limit by default

    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default player state class
    PlayerStateClass = APulseFirePlayerState::StaticClass();
}

void APulseFireGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Parse options
    UE_LOG(LogTemp, Warning, TEXT("InitGame: %s"), *Options);

    // Parse match duration
    if (UGameplayStatics::HasOption(Options, TEXT("MatchDuration")))
    {
        MatchDuration = FCString::Atof(*UGameplayStatics::ParseOption(Options, TEXT("MatchDuration")));
        MatchTimeRemaining = MatchDuration;
    }

    // Parse max players
    if (UGameplayStatics::HasOption(Options, TEXT("MaxPlayers")))
    {
        MaxPlayers = FCString::Atoi(*UGameplayStatics::ParseOption(Options, TEXT("MaxPlayers")));
    }

    // Parse score limit
    if (UGameplayStatics::HasOption(Options, TEXT("ScoreLimit")))
    {
        ScoreLimit = FCString::Atoi(*UGameplayStatics::ParseOption(Options, TEXT("ScoreLimit")));
    }
}

void APulseFireGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Set initial match state
    SetMatchState(FName("WaitingToStart"));
}

void APulseFireGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Update match timer if match is in progress
    if (MatchState == FName("InProgress"))
    {
        MatchTimeRemaining -= DeltaSeconds;
        
        // Check if match time has expired
        if (MatchTimeRemaining <= 0.0f)
        {
            MatchTimeRemaining = 0.0f;
            OnMatchTimeExpired();
        }
    }
}

void APulseFireGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate match state to all clients
    DOREPLIFETIME(APulseFireGameMode, MatchTimeRemaining);
    DOREPLIFETIME(APulseFireGameMode, MatchState);
}

void APulseFireGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Log player join
    UE_LOG(LogTemp, Warning, TEXT("Player joined: %s"), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName());

    // Check if we should start the match
    if (MatchState == FName("WaitingToStart"))
    {
        // Count connected players
        int32 NumPlayers = GetNumPlayers();
        UE_LOG(LogTemp, Warning, TEXT("Players connected: %d"), NumPlayers);

        // Start match if we have at least 2 players
        if (NumPlayers >= 2)
        {
            StartMatch();
        }
    }
    else if (MatchState == FName("InProgress"))
    {
        // Spawn the player
        RespawnPlayer(NewPlayer);
    }
}

void APulseFireGameMode::Logout(AController* Exiting)
{
    // Log player leave
    if (Exiting && Exiting->GetPlayerState<APlayerState>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player left: %s"), *Exiting->GetPlayerState<APlayerState>()->GetPlayerName());
    }

    Super::Logout(Exiting);

    // Check if we should end the match
    if (MatchState == FName("InProgress"))
    {
        // Count connected players
        int32 NumPlayers = GetNumPlayers();
        UE_LOG(LogTemp, Warning, TEXT("Players remaining: %d"), NumPlayers);

        // End match if we have less than 2 players
        if (NumPlayers < 2)
        {
            EndMatch();
        }
    }
}

void APulseFireGameMode::StartMatch()
{
    if (MatchState != FName("InProgress"))
    {
        // Reset match timer
        MatchTimeRemaining = MatchDuration;
        
        // Set match state
        SetMatchState(FName("InProgress"));
        
        // Spawn all players
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC && PC->GetPawn() == nullptr)
            {
                RespawnPlayer(PC);
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("Match started"));
    }
}

void APulseFireGameMode::EndMatch()
{
    if (MatchState == FName("InProgress"))
    {
        // Set match state
        SetMatchState(FName("MatchEnded"));
        
        // Broadcast match ended event
        OnMatchEndedEvent.Broadcast();

        UE_LOG(LogTemp, Warning, TEXT("Match ended"));
    }
}

void APulseFireGameMode::RestartMatch()
{
    // Reset match timer
    MatchTimeRemaining = MatchDuration;
    
    // Reset player scores
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            APulseFirePlayerState* PS = PC->GetPlayerState<APulseFirePlayerState>();
            if (PS)
            {
                PS->SetScore(0);
                OnPlayerScoreChangedEvent.Broadcast(PS, 0);
            }
        }
    }
    
    // Start the match
    StartMatch();
}

void APulseFireGameMode::OnMatchTimeExpired_Implementation()
{
    // End the match when time expires
    EndMatch();
}

void APulseFireGameMode::SetMatchState(FName NewState)
{
    if (MatchState != NewState)
    {
        MatchState = NewState;
        
        // Broadcast state change event
        OnMatchStateChangedEvent.Broadcast(NewState);
    }
}

void APulseFireGameMode::OnPlayerKilled(APulseFireCharacter* KilledPlayer, AController* KillerController)
{
    if (!KilledPlayer)
    {
        return;
    }

    // Broadcast player killed event
    OnPlayerKilledEvent.Broadcast(KilledPlayer);

    // Update killer's score
    if (KillerController && KillerController != KilledPlayer->GetController())
    {
        APulseFirePlayerState* KillerPS = KillerController->GetPlayerState<APulseFirePlayerState>();
        if (KillerPS)
        {
            // Add score for kill
            int32 NewScore = KillerPS->GetScore() + 1;
            KillerPS->SetScore(NewScore);
            
            // Broadcast score change event
            OnPlayerScoreChangedEvent.Broadcast(KillerPS, NewScore);
            
            // Check score limit
            if (ScoreLimit > 0 && NewScore >= ScoreLimit)
            {
                // End match if score limit reached
                EndMatch();
            }
        }
    }

    // Schedule respawn for killed player
    AController* KilledController = KilledPlayer->GetController();
    if (KilledController)
    {
        // Start respawn timer
        FTimerHandle& TimerHandle = RespawnTimers.FindOrAdd(KilledController);
        GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &APulseFireGameMode::RespawnPlayer, KilledController), RespawnDelay, false);
    }
}

void APulseFireGameMode::RespawnPlayer(AController* Controller)
{
    if (!Controller)
    {
        return;
    }

    // Clear respawn timer
    FTimerHandle* TimerHandle = RespawnTimers.Find(Controller);
    if (TimerHandle)
    {
        GetWorldTimerManager().ClearTimer(*TimerHandle);
        RespawnTimers.Remove(Controller);
    }

    // Destroy existing pawn
    if (Controller->GetPawn())
    {
        Controller->GetPawn()->Destroy();
    }

    // Get a random spawn point
    AActor* SpawnPoint = GetRandomSpawnPoint();
    if (SpawnPoint)
    {
        // Spawn the player
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        APulseFireCharacter* Character = GetWorld()->SpawnActor<APulseFireCharacter>(DefaultPawnClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), SpawnParams);
        if (Character)
        {
            // Possess the character
            Controller->Possess(Character);
            
            // Spawn default weapon
            Character->SpawnAndEquipWeapon(Character->DefaultWeaponType);
        }
    }
}

AActor* APulseFireGameMode::GetRandomSpawnPoint()
{
    // Get all player starts
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
    
    // Return random player start
    if (PlayerStarts.Num() > 0)
    {
        return PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
    }
    
    return nullptr;
}
