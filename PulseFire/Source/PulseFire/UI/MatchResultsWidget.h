#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "MatchResultsWidget.generated.h"

class UButton;
class UTextBlock;
class UScrollBox;
class UVerticalBox;
class UPlayerScoreWidget;

/**
 * Widget for displaying match results.
 */
UCLASS()
class PULSEFIRE_API UMatchResultsWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UMatchResultsWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Update the match results */
    UFUNCTION(BlueprintCallable, Category = "Match Results")
    void UpdateMatchResults();

protected:
    /** Match time text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MatchTimeText;

    /** Player scores scroll box */
    UPROPERTY(meta = (BindWidget))
    UScrollBox* PlayerScoresScrollBox;

    /** Continue button */
    UPROPERTY(meta = (BindWidget))
    UButton* ContinueButton;

    /** Main menu button */
    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    /** Player score widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPlayerScoreWidget> PlayerScoreWidgetClass;

    /** Continue button clicked */
    UFUNCTION()
    void OnContinueButtonClicked();

    /** Main menu button clicked */
    UFUNCTION()
    void OnMainMenuButtonClicked();
};
