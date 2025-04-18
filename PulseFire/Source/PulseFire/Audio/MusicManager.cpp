#include "MusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "SoundManager.h"

// Sets default values
AMusicManager::AMusicManager()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    CurrentMusicState = EMusicState::None;
    FadeTime = 2.0f;
    CombatTimeout = 10.0f;
}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Start with main menu music if we're in the main menu
    if (GetWorld()->GetName().Contains("MainMenu"))
    {
        SetMusicState(EMusicState::MainMenu);
    }
    else
    {
        SetMusicState(EMusicState::Gameplay);
    }
}

// Called every frame
void AMusicManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMusicManager::SetMusicState(EMusicState NewState)
{
    // Don't change if already in this state
    if (CurrentMusicState == NewState)
    {
        return;
    }

    // Update state
    CurrentMusicState = NewState;

    // Clear combat timer
    GetWorldTimerManager().ClearTimer(TimerHandle_Combat);

    // Get the appropriate music
    USoundBase* NewMusic = nullptr;
    switch (NewState)
    {
    case EMusicState::MainMenu:
        NewMusic = MainMenuMusic;
        break;
    case EMusicState::Gameplay:
        NewMusic = GameplayMusic;
        break;
    case EMusicState::Combat:
        NewMusic = CombatMusic;
        break;
    case EMusicState::Victory:
        NewMusic = VictoryMusic;
        break;
    case EMusicState::Defeat:
        NewMusic = DefeatMusic;
        break;
    default:
        break;
    }

    // Play the new music
    if (NewMusic)
    {
        // Stop current music
        if (CurrentMusicComponent)
        {
            USoundManager::StopMusic(CurrentMusicComponent, FadeTime);
        }

        // Play new music
        CurrentMusicComponent = USoundManager::PlayMusic(this, NewMusic, FadeTime);
    }
}

void AMusicManager::NotifyCombat()
{
    // Switch to combat music if not already in combat
    if (CurrentMusicState != EMusicState::Combat)
    {
        SetMusicState(EMusicState::Combat);
    }

    // Reset combat timer
    GetWorldTimerManager().SetTimer(TimerHandle_Combat, this, &AMusicManager::ResetToGameplayMusic, CombatTimeout, false);
}

void AMusicManager::ResetToGameplayMusic()
{
    // Switch back to gameplay music
    SetMusicState(EMusicState::Gameplay);
}

AMusicManager* AMusicManager::GetInstance(UObject* WorldContextObject)
{
    // Find the music manager in the world
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    // Find the first music manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AMusicManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        return Cast<AMusicManager>(FoundActors[0]);
    }

    // Spawn a new music manager if none exists
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<AMusicManager>(AMusicManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}
