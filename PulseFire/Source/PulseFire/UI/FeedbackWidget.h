#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "FeedbackWidget.generated.h"

class UButton;
class UEditableTextBox;
class UComboBoxString;
class USlider;
class UTextBlock;

/**
 * Widget for collecting user feedback.
 */
UCLASS()
class PULSEFIRE_API UFeedbackWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UFeedbackWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

protected:
    /** Feedback text box */
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* FeedbackTextBox;

    /** Category dropdown */
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* CategoryComboBox;

    /** Rating slider */
    UPROPERTY(meta = (BindWidget))
    USlider* RatingSlider;

    /** Rating text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* RatingText;

    /** Submit button */
    UPROPERTY(meta = (BindWidget))
    UButton* SubmitButton;

    /** Cancel button */
    UPROPERTY(meta = (BindWidget))
    UButton* CancelButton;

    /** Rating changed */
    UFUNCTION()
    void OnRatingChanged(float Value);

    /** Submit button clicked */
    UFUNCTION()
    void OnSubmitButtonClicked();

    /** Cancel button clicked */
    UFUNCTION()
    void OnCancelButtonClicked();

    /** Validate feedback */
    bool ValidateFeedback(FString& OutErrorMessage);

    /** Get rating text */
    FString GetRatingText(float Rating) const;
};
