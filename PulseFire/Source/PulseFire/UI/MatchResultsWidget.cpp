#include "MatchResultsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "../PulseFireGameInstance.h"
#include "../Multiplayer/PulseFireGameState.h"
#include "../Multiplayer/PulseFirePlayerState.h"
#include "PlayerScoreWidget.h"

UMatchResultsWidget::UMatchResultsWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMatchResultsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UMatchResultsWidget::OnContinueButtonClicked);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UMatchResultsWidget::OnMainMenuButtonClicked);
    }

    // Update match results
    UpdateMatchResults();
}

void UMatchResultsWidget::UpdateMatchResults()
{
    // Get game state
    APulseFireGameState* GameState = GetWorld()->GetGameState<APulseFireGameState>();
    if (!GameState)
    {
        return;
    }

    // Update match time
    if (MatchTimeText)
    {
        float MatchDuration = GameState->GetMatchDuration();
        int32 Minutes = FMath::FloorToInt(MatchDuration / 60.0f);
        int32 Seconds = FMath::FloorToInt(MatchDuration) % 60;
        FString TimeString = FString::Printf(TEXT("Match Time: %02d:%02d"), Minutes, Seconds);
        MatchTimeText->SetText(FText::FromString(TimeString));
    }

    // Clear player scores
    if (PlayerScoresScrollBox)
    {
        PlayerScoresScrollBox->ClearChildren();

        // Get all player states
        TArray<APlayerState*> PlayerStates;
        for (APlayerState* PlayerState : GameState->PlayerArray)
        {
            if (PlayerState)
            {
                PlayerStates.Add(PlayerState);
            }
        }

        // Sort player states by score
        PlayerStates.Sort([](const APlayerState& A, const APlayerState& B) {
            return A.GetScore() > B.GetScore();
        });

        // Add player score widgets
        for (APlayerState* PlayerState : PlayerStates)
        {
            APulseFirePlayerState* PulseFirePlayerState = Cast<APulseFirePlayerState>(PlayerState);
            if (PulseFirePlayerState && PlayerScoreWidgetClass)
            {
                UPlayerScoreWidget* PlayerScoreWidget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
                if (PlayerScoreWidget)
                {
                    PlayerScoreWidget->SetPlayerState(PulseFirePlayerState);
                    PlayerScoresScrollBox->AddChild(PlayerScoreWidget);
                }
            }
        }
    }
}

void UMatchResultsWidget::OnContinueButtonClicked()
{
    // Continue to next match
    // This would be implemented in the game-specific code
    // For now, just hide the widget
    HideWithAnimation();
}

void UMatchResultsWidget::OnMainMenuButtonClicked()
{
    // Return to main menu
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Destroy the current session
        GameInstance->DestroySession();
    }
}
