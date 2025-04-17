#include "PulseFireHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../PulseFireCharacter.h"
#include "../Weapons/BaseWeapon.h"
#include "../Multiplayer/PulseFireGameState.h"
#include "../Multiplayer/PulseFireGameMode.h"
#include "../Components/HealthComponent.h"

APulseFireHUD::APulseFireHUD()
{
    // Set default values
}

void APulseFireHUD::BeginPlay()
{
    Super::BeginPlay();

    // Create widgets
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(GetOwningPlayerController(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }

    if (MainMenuClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetOwningPlayerController(), MainMenuClass);
    }

    if (MultiplayerMenuClass)
    {
        MultiplayerMenuWidget = CreateWidget<UMultiplayerMenuWidget>(GetOwningPlayerController(), MultiplayerMenuClass);
    }

    if (PauseMenuClass)
    {
        PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetOwningPlayerController(), PauseMenuClass);
    }

    if (MatchResultsClass)
    {
        MatchResultsWidget = CreateWidget<UMatchResultsWidget>(GetOwningPlayerController(), MatchResultsClass);
    }

    // Register for game mode events
    APulseFireGameMode* GameMode = Cast<APulseFireGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnMatchStateChangedEvent.AddDynamic(this, &APulseFireHUD::OnMatchStateChanged);
        GameMode->OnMatchEndedEvent.AddDynamic(this, &APulseFireHUD::OnMatchEnded);
    }
}

void APulseFireHUD::DrawHUD()
{
    Super::DrawHUD();

    // Only draw HUD elements if we're not showing a menu and we have a HUD widget
    if (HUDWidget && HUDWidget->IsInViewport())
    {
        // Update HUD widget
        UpdateHUDWidget();
    }
    else if (!MainMenuWidget || !MainMenuWidget->IsInViewport())
    {
        // Draw HUD elements directly on canvas
        DrawHealthBar();
        DrawAmmoCounter();
        DrawMatchTimer();
        DrawCrosshair();
    }
}

void APulseFireHUD::ShowMainMenu()
{
    HideAllMenus();

    if (MainMenuWidget)
    {
        MainMenuWidget->ShowWithAnimation();

        // Show cursor and set input mode
        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
            PC->SetInputMode(InputMode);
        }
    }
}

void APulseFireHUD::ShowMultiplayerMenu()
{
    HideAllMenus();

    if (MultiplayerMenuWidget)
    {
        MultiplayerMenuWidget->ShowWithAnimation();

        // Show cursor and set input mode
        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MultiplayerMenuWidget->TakeWidget());
            PC->SetInputMode(InputMode);
        }
    }
}

void APulseFireHUD::ShowPauseMenu()
{
    HideAllMenus();

    if (PauseMenuWidget)
    {
        PauseMenuWidget->ShowWithAnimation();

        // Show cursor and set input mode
        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
            PC->SetInputMode(InputMode);

            // Pause the game
            PC->SetPause(true);
        }
    }
}

void APulseFireHUD::ShowMatchResults()
{
    HideAllMenus();

    if (MatchResultsWidget)
    {
        // Update match results
        MatchResultsWidget->UpdateMatchResults();
        MatchResultsWidget->ShowWithAnimation();

        // Show cursor and set input mode
        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MatchResultsWidget->TakeWidget());
            PC->SetInputMode(InputMode);
        }
    }
}

void APulseFireHUD::HideAllMenus()
{
    if (MainMenuWidget && MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->HideWithAnimation();
    }

    if (MultiplayerMenuWidget && MultiplayerMenuWidget->IsInViewport())
    {
        MultiplayerMenuWidget->HideWithAnimation();
    }

    if (PauseMenuWidget && PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->HideWithAnimation();

        // Unpause the game
        APlayerController* PC = GetOwningPlayerController();
        if (PC)
        {
            PC->SetPause(false);
        }
    }

    if (MatchResultsWidget && MatchResultsWidget->IsInViewport())
    {
        MatchResultsWidget->HideWithAnimation();
    }

    // Reset input mode
    APlayerController* PC = GetOwningPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = false;

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void APulseFireHUD::DrawHealthBar()
{
    // Get player character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Character)
    {
        return;
    }

    // Get health component
    UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
    if (!HealthComp)
    {
        return;
    }

    // Calculate health percentage
    float HealthPercent = HealthComp->GetHealthPercent();

    // Draw health bar
    float BarWidth = 200.0f;
    float BarHeight = 20.0f;
    float BarX = Canvas->SizeX * 0.1f;
    float BarY = Canvas->SizeY * 0.9f;

    // Draw background
    FCanvasBoxItem BoxItem(FVector2D(BarX, BarY), FVector2D(BarWidth, BarHeight));
    BoxItem.SetColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
    Canvas->DrawItem(BoxItem);

    // Draw health
    BoxItem.SetColor(FLinearColor(1.0f - HealthPercent, HealthPercent, 0.0f, 0.8f));
    BoxItem.Size = FVector2D(BarWidth * HealthPercent, BarHeight);
    Canvas->DrawItem(BoxItem);

    // Draw text
    FString HealthText = FString::Printf(TEXT("Health: %d%%"), FMath::RoundToInt(HealthPercent * 100.0f));
    FCanvasTextItem TextItem(FVector2D(BarX + 10.0f, BarY + 2.0f), FText::FromString(HealthText), HUDFont, FLinearColor::White);
    Canvas->DrawItem(TextItem);
}

void APulseFireHUD::DrawAmmoCounter()
{
    // Get player character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Character)
    {
        return;
    }

    // Get current weapon
    ABaseWeapon* Weapon = Character->CurrentWeapon;
    if (!Weapon)
    {
        return;
    }

    // Draw ammo counter
    FString AmmoText = FString::Printf(TEXT("%d / %d"), Weapon->GetCurrentAmmo(), Weapon->GetCurrentReserveAmmo());
    FCanvasTextItem TextItem(FVector2D(Canvas->SizeX * 0.9f, Canvas->SizeY * 0.9f), FText::FromString(AmmoText), HUDFont, FLinearColor::White);
    TextItem.Scale = FVector2D(1.5f, 1.5f);
    Canvas->DrawItem(TextItem);
}

void APulseFireHUD::DrawMatchTimer()
{
    // Get game state
    APulseFireGameState* GameState = GetWorld()->GetGameState<APulseFireGameState>();
    if (!GameState || !GameState->IsMatchInProgress())
    {
        return;
    }

    // Calculate minutes and seconds
    int32 Minutes = FMath::FloorToInt(GameState->GetMatchTimeRemaining() / 60.0f);
    int32 Seconds = FMath::FloorToInt(GameState->GetMatchTimeRemaining()) % 60;

    // Draw timer
    FString TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    FCanvasTextItem TextItem(FVector2D(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.1f), FText::FromString(TimerText), HUDFont, FLinearColor::White);
    TextItem.Scale = FVector2D(2.0f, 2.0f);
    TextItem.bCentreX = true;
    Canvas->DrawItem(TextItem);
}

void APulseFireHUD::DrawCrosshair()
{
    // Get player character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Character)
    {
        return;
    }

    // Get current weapon
    ABaseWeapon* Weapon = Character->CurrentWeapon;
    if (!Weapon)
    {
        return;
    }

    // Calculate crosshair size based on weapon spread
    float CrosshairSize = 10.0f;
    if (Weapon->IsAiming())
    {
        CrosshairSize = 5.0f;
    }

    // Draw crosshair
    float CenterX = Canvas->SizeX * 0.5f;
    float CenterY = Canvas->SizeY * 0.5f;

    // Draw lines
    FCanvasLineItem LineItem(FVector2D(CenterX - CrosshairSize, CenterY), FVector2D(CenterX + CrosshairSize, CenterY));
    LineItem.SetColor(FLinearColor::White);
    Canvas->DrawItem(LineItem);

    LineItem.Origin = FVector2D(CenterX, CenterY - CrosshairSize);
    LineItem.EndPos = FVector2D(CenterX, CenterY + CrosshairSize);
    Canvas->DrawItem(LineItem);
}

void APulseFireHUD::UpdateHUDWidget()
{
    if (!HUDWidget)
    {
        return;
    }

    // Get player character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Character)
    {
        return;
    }

    // Update health
    UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
    if (HealthComp)
    {
        HUDWidget->UpdateHealth(HealthComp->GetHealthPercent());
    }

    // Update ammo
    ABaseWeapon* Weapon = Character->CurrentWeapon;
    if (Weapon)
    {
        HUDWidget->UpdateAmmo(Weapon->GetCurrentAmmo(), Weapon->GetMaxAmmo(), Weapon->GetCurrentReserveAmmo());
        HUDWidget->UpdateWeaponName(Weapon->GetWeaponName());
    }

    // Update match timer
    APulseFireGameState* GameState = GetWorld()->GetGameState<APulseFireGameState>();
    if (GameState)
    {
        HUDWidget->UpdateMatchTimer(GameState->GetMatchTimeRemaining(), GameState->GetMatchDuration());
    }
}

void APulseFireHUD::OnMatchStateChanged(FName NewState)
{
    // Handle match state changes
    if (NewState == FName("MatchEnded"))
    {
        // Show match results
        ShowMatchResults();
    }
    else if (NewState == FName("WaitingToStart"))
    {
        // Show main menu
        ShowMainMenu();
    }
}

void APulseFireHUD::OnMatchEnded()
{
    // Show match results
    ShowMatchResults();
}
