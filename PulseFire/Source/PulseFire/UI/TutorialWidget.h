#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "TutorialManager.h"
#include "TutorialWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;

/**
 * Widget for displaying tutorial steps.
 */
UCLASS()
class PULSEFIRE_API UTutorialWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UTutorialWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Update the tutorial step */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void UpdateTutorialStep(const FTutorialStep& Step);

protected:
    /** Title text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TitleText;

    /** Description text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DescriptionText;

    /** Tutorial image */
    UPROPERTY(meta = (BindWidget))
    UImage* TutorialImage;

    /** Next button */
    UPROPERTY(meta = (BindWidget))
    UButton* NextButton;

    /** Previous button */
    UPROPERTY(meta = (BindWidget))
    UButton* PreviousButton;

    /** Skip button */
    UPROPERTY(meta = (BindWidget))
    UButton* SkipButton;

    /** Next button clicked */
    UFUNCTION()
    void OnNextButtonClicked();

    /** Previous button clicked */
    UFUNCTION()
    void OnPreviousButtonClicked();

    /** Skip button clicked */
    UFUNCTION()
    void OnSkipButtonClicked();
};
