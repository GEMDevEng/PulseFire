#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/WidgetAnimation.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize widget components
    if (HealthBar)
    {
        HealthBar->SetPercent(1.0f);
    }

    if (HealthText)
    {
        HealthText->SetText(FText::FromString("100"));
    }

    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString("30"));
    }

    if (ReserveAmmoText)
    {
        ReserveAmmoText->SetText(FText::FromString("90"));
    }

    if (MatchTimerText)
    {
        MatchTimerText->SetText(FText::FromString("05:00"));
    }

    if (WeaponNameText)
    {
        WeaponNameText->SetText(FText::FromString("Assault Rifle"));
    }

    // Hide hit markers and damage indicators
    if (HitMarkerImage)
    {
        HitMarkerImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (HeadshotMarkerImage)
    {
        HeadshotMarkerImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (DamageIndicatorImage)
    {
        DamageIndicatorImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHUDWidget::UpdateHealth(float HealthPercent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercent);
    }

    if (HealthText)
    {
        int32 HealthValue = FMath::RoundToInt(HealthPercent * 100.0f);
        HealthText->SetText(FText::FromString(FString::FromInt(HealthValue)));
    }
}

void UHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo, int32 ReserveAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
    }

    if (ReserveAmmoText)
    {
        ReserveAmmoText->SetText(FText::FromString(FString::FromInt(ReserveAmmo)));
    }
}

void UHUDWidget::UpdateMatchTimer(float RemainingTime, float TotalTime)
{
    if (MatchTimerText)
    {
        MatchTimerText->SetText(FText::FromString(FormatTime(RemainingTime)));
    }
}

void UHUDWidget::UpdateWeaponName(const FString& WeaponName)
{
    if (WeaponNameText)
    {
        WeaponNameText->SetText(FText::FromString(WeaponName));
    }
}

void UHUDWidget::ShowHitMarker(bool bIsHeadshot)
{
    // Clear any existing timer
    GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimerHandle);

    if (bIsHeadshot)
    {
        if (HeadshotMarkerImage)
        {
            HeadshotMarkerImage->SetVisibility(ESlateVisibility::Visible);
        }

        if (HeadshotMarkerAnimation)
        {
            PlayAnimation(HeadshotMarkerAnimation);
        }
    }
    else
    {
        if (HitMarkerImage)
        {
            HitMarkerImage->SetVisibility(ESlateVisibility::Visible);
        }

        if (HitMarkerAnimation)
        {
            PlayAnimation(HitMarkerAnimation);
        }
    }

    // Set timer to hide hit marker
    GetWorld()->GetTimerManager().SetTimer(
        HitMarkerTimerHandle,
        this,
        &UHUDWidget::HideHitMarker,
        0.5f,
        false
    );
}

void UHUDWidget::ShowDamageIndicator(const FVector& DamageDirection)
{
    // Clear any existing timer
    GetWorld()->GetTimerManager().ClearTimer(DamageIndicatorTimerHandle);

    if (DamageIndicatorImage)
    {
        // Calculate rotation based on damage direction
        APlayerController* PlayerController = GetOwningPlayer();
        if (PlayerController)
        {
            FVector PlayerLocation;
            FRotator PlayerRotation;
            PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

            // Calculate angle between player forward and damage direction
            FVector PlayerForward = PlayerRotation.Vector();
            FVector FlatDamageDir = DamageDirection;
            FlatDamageDir.Z = 0.0f;
            FlatDamageDir.Normalize();

            float DotProduct = FVector::DotProduct(PlayerForward, FlatDamageDir);
            float CrossProduct = FVector::CrossProduct(PlayerForward, FlatDamageDir).Z;
            float Angle = FMath::Acos(DotProduct);
            if (CrossProduct < 0.0f)
            {
                Angle = -Angle;
            }

            // Convert to degrees
            float Degrees = FMath::RadiansToDegrees(Angle);

            // Set rotation of damage indicator
            DamageIndicatorImage->SetRenderTransformAngle(Degrees);
            DamageIndicatorImage->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if (DamageIndicatorAnimation)
    {
        PlayAnimation(DamageIndicatorAnimation);
    }

    // Set timer to hide damage indicator
    GetWorld()->GetTimerManager().SetTimer(
        DamageIndicatorTimerHandle,
        this,
        &UHUDWidget::HideDamageIndicator,
        1.0f,
        false
    );
}

void UHUDWidget::AddKillFeedEntry(const FString& KillerName, const FString& VictimName, const FString& WeaponName)
{
    // This would be implemented in Blueprint to add a kill feed entry widget to a vertical box
}

void UHUDWidget::HideHitMarker()
{
    if (HitMarkerImage)
    {
        HitMarkerImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (HeadshotMarkerImage)
    {
        HeadshotMarkerImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UHUDWidget::HideDamageIndicator()
{
    if (DamageIndicatorImage)
    {
        DamageIndicatorImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

FString UHUDWidget::FormatTime(float TimeInSeconds)
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
