#include "TutorialManager.h"
#include "TutorialWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../Audio/SoundManager.h"

// Sets default values
ATutorialManager::ATutorialManager()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    CurrentStepIndex = 0;
    bTutorialActive = false;
    bTutorialPaused = false;
}

// Called when the game starts or when spawned
void ATutorialManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Create tutorial widget
    if (TutorialWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            TutorialWidget = CreateWidget<UTutorialWidget>(PC, TutorialWidgetClass);
        }
    }

    // Start tutorial automatically if this is the first time playing
    // In a real game, we would check if the player has completed the tutorial before
    bool bFirstTimePlayer = true;
    if (bFirstTimePlayer)
    {
        StartTutorial();
    }
}

// Called every frame
void ATutorialManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATutorialManager::StartTutorial()
{
    // Reset tutorial state
    CurrentStepIndex = 0;
    bTutorialActive = true;
    bTutorialPaused = false;

    // Reset tutorial steps
    for (FTutorialStep& Step : TutorialSteps)
    {
        Step.bCompleted = false;
    }

    // Show first step
    ShowCurrentStep();
}

void ATutorialManager::PauseTutorial()
{
    if (bTutorialActive && !bTutorialPaused)
    {
        bTutorialPaused = true;

        // Pause auto-advance timer
        if (GetWorldTimerManager().IsTimerActive(TimerHandle_NextStep))
        {
            GetWorldTimerManager().PauseTimer(TimerHandle_NextStep);
        }

        // Hide tutorial widget
        if (TutorialWidget && TutorialWidget->IsInViewport())
        {
            TutorialWidget->RemoveFromViewport();
        }
    }
}

void ATutorialManager::ResumeTutorial()
{
    if (bTutorialActive && bTutorialPaused)
    {
        bTutorialPaused = false;

        // Resume auto-advance timer
        if (GetWorldTimerManager().IsTimerPaused(TimerHandle_NextStep))
        {
            GetWorldTimerManager().UnPauseTimer(TimerHandle_NextStep);
        }

        // Show current step
        ShowCurrentStep();
    }
}

void ATutorialManager::SkipTutorial()
{
    // Complete all steps
    for (FTutorialStep& Step : TutorialSteps)
    {
        Step.bCompleted = true;
    }

    // Complete tutorial
    CompleteTutorial();
}

void ATutorialManager::NextStep()
{
    // Mark current step as completed
    if (CurrentStepIndex >= 0 && CurrentStepIndex < TutorialSteps.Num())
    {
        TutorialSteps[CurrentStepIndex].bCompleted = true;
    }

    // Clear timer
    GetWorldTimerManager().ClearTimer(TimerHandle_NextStep);

    // Advance to next step
    CurrentStepIndex++;

    // Check if tutorial is complete
    if (CurrentStepIndex >= TutorialSteps.Num())
    {
        CompleteTutorial();
        return;
    }

    // Show next step
    ShowCurrentStep();
}

void ATutorialManager::PreviousStep()
{
    // Clear timer
    GetWorldTimerManager().ClearTimer(TimerHandle_NextStep);

    // Go back to previous step
    CurrentStepIndex = FMath::Max(0, CurrentStepIndex - 1);

    // Show previous step
    ShowCurrentStep();
}

void ATutorialManager::ShowCurrentStep()
{
    if (!bTutorialActive || bTutorialPaused || CurrentStepIndex < 0 || CurrentStepIndex >= TutorialSteps.Num())
    {
        return;
    }

    // Get current step
    const FTutorialStep& CurrentStep = TutorialSteps[CurrentStepIndex];

    // Update tutorial widget
    if (TutorialWidget)
    {
        // Add to viewport if not already
        if (!TutorialWidget->IsInViewport())
        {
            TutorialWidget->AddToViewport();
        }

        // Update widget
        TutorialWidget->UpdateTutorialStep(CurrentStep);
    }

    // Broadcast step changed event
    OnTutorialStepChanged.Broadcast(CurrentStep);

    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Set timer for auto-advance if needed
    if (CurrentStep.Duration > 0.0f && !CurrentStep.bRequiresInput)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_NextStep, this, &ATutorialManager::NextStep, CurrentStep.Duration, false);
    }
}

void ATutorialManager::HideTutorial()
{
    // Hide tutorial widget
    if (TutorialWidget && TutorialWidget->IsInViewport())
    {
        TutorialWidget->RemoveFromViewport();
    }

    // Clear timer
    GetWorldTimerManager().ClearTimer(TimerHandle_NextStep);
}

void ATutorialManager::CompleteTutorial()
{
    // Hide tutorial
    HideTutorial();

    // Set tutorial as inactive
    bTutorialActive = false;
    bTutorialPaused = false;

    // Broadcast tutorial completed event
    OnTutorialCompleted.Broadcast();
}

void ATutorialManager::HandleInputAction(FName ActionName)
{
    if (!bTutorialActive || bTutorialPaused || CurrentStepIndex < 0 || CurrentStepIndex >= TutorialSteps.Num())
    {
        return;
    }

    // Get current step
    const FTutorialStep& CurrentStep = TutorialSteps[CurrentStepIndex];

    // Check if this input action completes the current step
    if (CurrentStep.bRequiresInput && CurrentStep.RequiredInputAction == ActionName)
    {
        NextStep();
    }
}

FTutorialStep ATutorialManager::GetCurrentStep() const
{
    if (CurrentStepIndex >= 0 && CurrentStepIndex < TutorialSteps.Num())
    {
        return TutorialSteps[CurrentStepIndex];
    }

    // Return empty step if invalid
    return FTutorialStep();
}

ATutorialManager* ATutorialManager::GetInstance(UObject* WorldContextObject)
{
    // Find the tutorial manager in the world
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    // Find the first tutorial manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, ATutorialManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        return Cast<ATutorialManager>(FoundActors[0]);
    }

    // Spawn a new tutorial manager if none exists
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<ATutorialManager>(ATutorialManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}
