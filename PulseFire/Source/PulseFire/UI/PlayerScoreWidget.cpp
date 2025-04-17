#include "PlayerScoreWidget.h"
#include "Components/TextBlock.h"
#include "../Multiplayer/PulseFirePlayerState.h"

UPlayerScoreWidget::UPlayerScoreWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UPlayerScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Update the widget
    UpdateWidget();
}

void UPlayerScoreWidget::SetPlayerState(APulseFirePlayerState* InPlayerState)
{
    PlayerState = InPlayerState;
    UpdateWidget();
}

void UPlayerScoreWidget::UpdateWidget()
{
    if (!PlayerState)
    {
        return;
    }

    // Update player name
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
    }

    // Update score
    if (ScoreText)
    {
        ScoreText->SetText(FText::AsNumber(FMath::RoundToInt(PlayerState->GetScore())));
    }

    // Update kills
    if (KillsText)
    {
        KillsText->SetText(FText::AsNumber(PlayerState->GetKills()));
    }

    // Update deaths
    if (DeathsText)
    {
        DeathsText->SetText(FText::AsNumber(PlayerState->GetDeaths()));
    }

    // Update K/D ratio
    if (KDRatioText)
    {
        float KDRatio = PlayerState->GetKDRatio();
        KDRatioText->SetText(FText::AsNumber(KDRatio));
    }
}
