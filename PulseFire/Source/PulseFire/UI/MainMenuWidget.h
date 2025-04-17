#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class UWidgetSwitcher;

/**
 * Widget for the main menu.
 */
UCLASS()
class PULSEFIRE_API UMainMenuWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

protected:
    /** Main menu container */
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* MenuSwitcher;

    /** Main menu panel */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* MainMenuPanel;

    /** Multiplayer menu panel */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* MultiplayerMenuPanel;

    /** Options menu panel */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* OptionsMenuPanel;

    /** Credits menu panel */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* CreditsMenuPanel;

    /** Play button */
    UPROPERTY(meta = (BindWidget))
    UButton* PlayButton;

    /** Options button */
    UPROPERTY(meta = (BindWidget))
    UButton* OptionsButton;

    /** Credits button */
    UPROPERTY(meta = (BindWidget))
    UButton* CreditsButton;

    /** Quit button */
    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    /** Host game button */
    UPROPERTY(meta = (BindWidget))
    UButton* HostGameButton;

    /** Join game button */
    UPROPERTY(meta = (BindWidget))
    UButton* JoinGameButton;

    /** Find games button */
    UPROPERTY(meta = (BindWidget))
    UButton* FindGamesButton;

    /** Back button (multiplayer) */
    UPROPERTY(meta = (BindWidget))
    UButton* BackFromMultiplayerButton;

    /** Back button (options) */
    UPROPERTY(meta = (BindWidget))
    UButton* BackFromOptionsButton;

    /** Back button (credits) */
    UPROPERTY(meta = (BindWidget))
    UButton* BackFromCreditsButton;

    /** Play button clicked */
    UFUNCTION()
    void OnPlayButtonClicked();

    /** Options button clicked */
    UFUNCTION()
    void OnOptionsButtonClicked();

    /** Credits button clicked */
    UFUNCTION()
    void OnCreditsButtonClicked();

    /** Quit button clicked */
    UFUNCTION()
    void OnQuitButtonClicked();

    /** Host game button clicked */
    UFUNCTION()
    void OnHostGameButtonClicked();

    /** Join game button clicked */
    UFUNCTION()
    void OnJoinGameButtonClicked();

    /** Find games button clicked */
    UFUNCTION()
    void OnFindGamesButtonClicked();

    /** Back button clicked (multiplayer) */
    UFUNCTION()
    void OnBackFromMultiplayerButtonClicked();

    /** Back button clicked (options) */
    UFUNCTION()
    void OnBackFromOptionsButtonClicked();

    /** Back button clicked (credits) */
    UFUNCTION()
    void OnBackFromCreditsButtonClicked();
};
