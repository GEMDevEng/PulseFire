#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "SniperRifle.generated.h"

/**
 * Sniper rifle weapon class.
 * High damage, low rate of fire, high accuracy.
 */
UCLASS()
class PULSEFIRE_API ASniperRifle : public ABaseWeapon
{
    GENERATED_BODY()
    
public:
    // Sets default values
    ASniperRifle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Scope overlay material */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    UMaterialInterface* ScopeMaterial;

    /** Scope overlay widget */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    TSubclassOf<UUserWidget> ScopeWidgetClass;

    /** Scope widget instance */
    UPROPERTY()
    UUserWidget* ScopeWidget;

    /** Scope FOV */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    float ScopeFOV;

    /** Default FOV */
    UPROPERTY()
    float DefaultFOV;

    /** Whether the weapon is scoped */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon|Sniper")
    bool bIsScoped;

    /** Scope-in time */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    float ScopeInTime;

    /** Scope-out time */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    float ScopeOutTime;

    /** Breath hold time (seconds) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    float BreathHoldTime;

    /** Current breath hold time remaining */
    UPROPERTY(BlueprintReadOnly, Category = "Weapon|Sniper")
    float CurrentBreathHoldTime;

    /** Sway reduction when holding breath */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sniper")
    float BreathHoldSwayReduction;

    /** Whether the player is holding breath */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon|Sniper")
    bool bIsHoldingBreath;

    /** Timer handle for scope transition */
    FTimerHandle TimerHandle_ScopeTransition;

    /** Timer handle for breath hold */
    FTimerHandle TimerHandle_BreathHold;

    /** Show scope overlay */
    virtual void ShowScopeOverlay();

    /** Hide scope overlay */
    virtual void HideScopeOverlay();

    /** Start holding breath */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    virtual void StartHoldBreath();

    /** Stop holding breath */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    virtual void StopHoldBreath();

    /** Reset breath */
    UFUNCTION()
    virtual void ResetBreath();

    /** Server-side hold breath implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetHoldingBreath(bool bNewHoldingBreath);
    bool ServerSetHoldingBreath_Validate(bool bNewHoldingBreath);
    void ServerSetHoldingBreath_Implementation(bool bNewHoldingBreath);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Set aiming state */
    virtual void SetAiming(bool bNewAiming) override;

    /** Get whether the weapon is scoped */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    bool IsScoped() const { return bIsScoped; }

    /** Get whether the player is holding breath */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    bool IsHoldingBreath() const { return bIsHoldingBreath; }

    /** Get current breath hold time remaining */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    float GetCurrentBreathHoldTime() const { return CurrentBreathHoldTime; }

    /** Get breath hold time */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    float GetBreathHoldTime() const { return BreathHoldTime; }

    /** Get current sway reduction */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Sniper")
    float GetCurrentSwayReduction() const;
};
