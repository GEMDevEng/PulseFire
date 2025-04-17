#include "TutorialWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TutorialManager.h"
#include "../Audio/SoundManager.h"

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UTutorialWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (NextButton)
    {
        NextButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnNextButtonClicked);
    }

    if (PreviousButton)
    {
        PreviousButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnPreviousButtonClicked);
    }

    if (SkipButton)
    {
        SkipButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnSkipButtonClicked);
    }
}

void UTutorialWidget::UpdateTutorialStep(const FTutorialStep& Step)
{
    // Update title
    if (TitleText)
    {
        TitleText->SetText(Step.Title);
    }

    // Update description
    if (DescriptionText)
    {
        DescriptionText->SetText(Step.Description);
    }

    // Update image
    if (TutorialImage && Step.Image)
    {
        TutorialImage->SetBrushFromTexture(Step.Image);
        TutorialImage->SetVisibility(ESlateVisibility::Visible);
    }
    else if (TutorialImage)
    {
        TutorialImage->SetVisibility(ESlateVisibility::Hidden);
    }

    // Update previous button visibility
    if (PreviousButton)
    {
        ATutorialManager* TutorialManager = ATutorialManager::GetInstance(this);
        if (TutorialManager)
        {
            // Hide previous button on first step
            PreviousButton->SetVisibility(TutorialManager->GetCurrentStepIndex() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}

void UTutorialWidget::OnNextButtonClicked()
{
    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Advance to next step
    ATutorialManager* TutorialManager = ATutorialManager::GetInstance(this);
    if (TutorialManager)
    {
        TutorialManager->NextStep();
    }
}

void UTutorialWidget::OnPreviousButtonClicked()
{
    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Go back to previous step
    ATutorialManager* TutorialManager = ATutorialManager::GetInstance(this);
    if (TutorialManager)
    {
        TutorialManager->PreviousStep();
    }
}

void UTutorialWidget::OnSkipButtonClicked()
{
    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Skip tutorial
    ATutorialManager* TutorialManager = ATutorialManager::GetInstance(this);
    if (TutorialManager)
    {
        TutorialManager->SkipTutorial();
    }
}
