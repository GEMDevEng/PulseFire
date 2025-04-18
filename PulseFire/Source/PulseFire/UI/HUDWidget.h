#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

/**
 * Widget for displaying in-game HUD information.
 */
UCLASS()
class PULSEFIRE_API UHUDWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UHUDWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Called every frame */
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    /** Update health display */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float HealthPercent);

    /** Update ammo display */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo, int32 ReserveAmmo);

    /** Update match timer display */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateMatchTimer(float RemainingTime, float TotalTime);

    /** Update weapon name display */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWeaponName(const FString& WeaponName);

    /** Show hit marker */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowHitMarker(bool bIsHeadshot = false);

    /** Show damage indicator */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowDamageIndicator(const FVector& DamageDirection);

    /** Show kill feed entry */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void AddKillFeedEntry(const FString& KillerName, const FString& VictimName, const FString& WeaponName);

protected:
    /** Health bar */
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    /** Health text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;

    /** Ammo text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmmoText;

    /** Reserve ammo text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ReserveAmmoText;

    /** Match timer text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MatchTimerText;

    /** Weapon name text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponNameText;

    /** Crosshair image */
    UPROPERTY(meta = (BindWidget))
    UImage* CrosshairImage;

    /** Hit marker image */
    UPROPERTY(meta = (BindWidget))
    UImage* HitMarkerImage;

    /** Headshot hit marker image */
    UPROPERTY(meta = (BindWidget))
    UImage* HeadshotMarkerImage;

    /** Damage indicator image */
    UPROPERTY(meta = (BindWidget))
    UImage* DamageIndicatorImage;

    /** Animation to play when showing hit marker */
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HitMarkerAnimation;

    /** Animation to play when showing headshot marker */
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HeadshotMarkerAnimation;

    /** Animation to play when showing damage indicator */
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* DamageIndicatorAnimation;

    /** Timer handle for hit marker */
    FTimerHandle HitMarkerTimerHandle;

    /** Timer handle for damage indicator */
    FTimerHandle DamageIndicatorTimerHandle;

    /** Hide hit marker */
    UFUNCTION()
    void HideHitMarker();

    /** Hide damage indicator */
    UFUNCTION()
    void HideDamageIndicator();

    /** Format time as MM:SS */
    FString FormatTime(float TimeInSeconds);
};
