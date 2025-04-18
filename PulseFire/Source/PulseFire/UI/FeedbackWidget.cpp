#include "FeedbackWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "../Analytics/AnalyticsManager.h"
#include "../Audio/SoundManager.h"

UFeedbackWidget::UFeedbackWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UFeedbackWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (SubmitButton)
    {
        SubmitButton->OnClicked.AddDynamic(this, &UFeedbackWidget::OnSubmitButtonClicked);
    }

    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &UFeedbackWidget::OnCancelButtonClicked);
    }

    // Bind slider value changed event
    if (RatingSlider)
    {
        RatingSlider->OnValueChanged.AddDynamic(this, &UFeedbackWidget::OnRatingChanged);
        
        // Set initial rating text
        OnRatingChanged(RatingSlider->GetValue());
    }

    // Add categories to combo box
    if (CategoryComboBox)
    {
        CategoryComboBox->AddOption(TEXT("General"));
        CategoryComboBox->AddOption(TEXT("Gameplay"));
        CategoryComboBox->AddOption(TEXT("Graphics"));
        CategoryComboBox->AddOption(TEXT("Audio"));
        CategoryComboBox->AddOption(TEXT("Performance"));
        CategoryComboBox->AddOption(TEXT("UI/UX"));
        CategoryComboBox->AddOption(TEXT("Bug Report"));
        CategoryComboBox->AddOption(TEXT("Feature Request"));
        CategoryComboBox->AddOption(TEXT("Other"));
        
        // Set default selection
        CategoryComboBox->SetSelectedIndex(0);
    }
}

void UFeedbackWidget::OnRatingChanged(float Value)
{
    // Update rating text
    if (RatingText)
    {
        int32 Rating = FMath::RoundToInt(Value * 5.0f);
        RatingText->SetText(FText::FromString(GetRatingText(Rating)));
    }
}

void UFeedbackWidget::OnSubmitButtonClicked()
{
    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Validate feedback
    FString ErrorMessage;
    if (!ValidateFeedback(ErrorMessage))
    {
        // Show error message
        // In a real implementation, we would show an error message to the user
        return;
    }

    // Get feedback data
    FString FeedbackText = FeedbackTextBox->GetText().ToString();
    FString Category = CategoryComboBox->GetSelectedOption();
    int32 Rating = FMath::RoundToInt(RatingSlider->GetValue() * 5.0f);

    // Submit feedback
    UAnalyticsManager* AnalyticsManager = UAnalyticsManager::GetInstance();
    if (AnalyticsManager)
    {
        AnalyticsManager->SubmitFeedback(FeedbackText, Rating, Category);
    }

    // Show thank you message
    // In a real implementation, we would show a thank you message to the user

    // Close widget
    RemoveFromParent();
}

void UFeedbackWidget::OnCancelButtonClicked()
{
    // Play UI sound
    USoundManager::PlayUISound(nullptr); // Replace with actual sound

    // Close widget
    RemoveFromParent();
}

bool UFeedbackWidget::ValidateFeedback(FString& OutErrorMessage)
{
    // Check if feedback text is empty
    if (FeedbackTextBox->GetText().IsEmpty())
    {
        OutErrorMessage = TEXT("Please enter your feedback.");
        return false;
    }

    // Check if feedback text is too short
    if (FeedbackTextBox->GetText().ToString().Len() < 10)
    {
        OutErrorMessage = TEXT("Please provide more detailed feedback.");
        return false;
    }

    // Check if category is selected
    if (CategoryComboBox->GetSelectedIndex() == -1)
    {
        OutErrorMessage = TEXT("Please select a category.");
        return false;
    }

    return true;
}

FString UFeedbackWidget::GetRatingText(float Rating) const
{
    switch (FMath::RoundToInt(Rating))
    {
    case 0:
        return TEXT("Very Poor (0/5)");
    case 1:
        return TEXT("Poor (1/5)");
    case 2:
        return TEXT("Average (2/5)");
    case 3:
        return TEXT("Good (3/5)");
    case 4:
        return TEXT("Very Good (4/5)");
    case 5:
        return TEXT("Excellent (5/5)");
    default:
        return TEXT("Rating");
    }
}
