#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PulseFireHUD.generated.h"

/**
 * HUD class for PulseFire.
 * Displays game information like health, ammo, and match time.
 */
UCLASS()
class PULSEFIRE_API APulseFireHUD : public AHUD
{
    GENERATED_BODY()
    
public:
    APulseFireHUD();

    /** Initialize the HUD */
    virtual void BeginPlay() override;

    /** Draw the HUD */
    virtual void DrawHUD() override;

    /** Show the main menu */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMainMenu();

    /** Show the multiplayer menu */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMultiplayerMenu();

    /** Show the pause menu */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowPauseMenu();

    /** Show the match results */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMatchResults();

    /** Hide all menus */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideAllMenus();

protected:
    /** Font for HUD text */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    UFont* HUDFont;

    /** Main menu widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MainMenuClass;

    /** Multiplayer menu widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MultiplayerMenuClass;

    /** Pause menu widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuClass;

    /** Match results widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MatchResultsClass;

    /** HUD widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    /** Main menu widget instance */
    UPROPERTY()
    class UUserWidget* MainMenuWidget;

    /** Multiplayer menu widget instance */
    UPROPERTY()
    class UUserWidget* MultiplayerMenuWidget;

    /** Pause menu widget instance */
    UPROPERTY()
    class UUserWidget* PauseMenuWidget;

    /** Match results widget instance */
    UPROPERTY()
    class UUserWidget* MatchResultsWidget;

    /** HUD widget instance */
    UPROPERTY()
    class UUserWidget* HUDWidget;

    /** Draw health bar */
    void DrawHealthBar();

    /** Draw ammo counter */
    void DrawAmmoCounter();

    /** Draw match timer */
    void DrawMatchTimer();

    /** Draw crosshair */
    void DrawCrosshair();
};
