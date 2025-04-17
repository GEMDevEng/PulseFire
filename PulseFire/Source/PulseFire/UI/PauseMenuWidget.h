#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

/**
 * Widget for the pause menu.
 */
UCLASS()
class PULSEFIRE_API UPauseMenuWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

protected:
    /** Resume button */
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    /** Options button */
    UPROPERTY(meta = (BindWidget))
    UButton* OptionsButton;

    /** Main menu button */
    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    /** Quit button */
    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    /** Resume button clicked */
    UFUNCTION()
    void OnResumeButtonClicked();

    /** Options button clicked */
    UFUNCTION()
    void OnOptionsButtonClicked();

    /** Main menu button clicked */
    UFUNCTION()
    void OnMainMenuButtonClicked();

    /** Quit button clicked */
    UFUNCTION()
    void OnQuitButtonClicked();
};
