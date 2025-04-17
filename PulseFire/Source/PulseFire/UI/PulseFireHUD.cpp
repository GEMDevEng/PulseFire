#include "PulseFireHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../PulseFireCharacter.h"
#include "../Weapons/BaseWeapon.h"
#include "../Multiplayer/PulseFireGameState.h"
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
        HUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }

    if (MainMenuClass)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuClass);
    }

    if (MultiplayerMenuClass)
    {
        MultiplayerMenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MultiplayerMenuClass);
    }

    if (PauseMenuClass)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), PauseMenuClass);
    }

    if (MatchResultsClass)
    {
        MatchResultsWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MatchResultsClass);
    }
}

void APulseFireHUD::DrawHUD()
{
    Super::DrawHUD();

    // Only draw HUD elements if we're not showing a menu
    if (!MainMenuWidget || !MainMenuWidget->IsInViewport())
    {
        // Draw HUD elements
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
        MainMenuWidget->AddToViewport();
    }
}

void APulseFireHUD::ShowMultiplayerMenu()
{
    HideAllMenus();

    if (MultiplayerMenuWidget)
    {
        MultiplayerMenuWidget->AddToViewport();
    }
}

void APulseFireHUD::ShowPauseMenu()
{
    HideAllMenus();

    if (PauseMenuWidget)
    {
        PauseMenuWidget->AddToViewport();
    }
}

void APulseFireHUD::ShowMatchResults()
{
    HideAllMenus();

    if (MatchResultsWidget)
    {
        MatchResultsWidget->AddToViewport();
    }
}

void APulseFireHUD::HideAllMenus()
{
    if (MainMenuWidget && MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->RemoveFromViewport();
    }

    if (MultiplayerMenuWidget && MultiplayerMenuWidget->IsInViewport())
    {
        MultiplayerMenuWidget->RemoveFromViewport();
    }

    if (PauseMenuWidget && PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->RemoveFromViewport();
    }

    if (MatchResultsWidget && MatchResultsWidget->IsInViewport())
    {
        MatchResultsWidget->RemoveFromViewport();
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
