#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PostProcessManager.generated.h"

class UMaterialInstanceDynamic;
class UPostProcessComponent;

/**
 * Component for managing post-processing effects.
 */
UCLASS(ClassGroup=(VFX), meta=(BlueprintSpawnableComponent))
class PULSEFIRE_API UPostProcessManager : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UPostProcessManager();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    /** Post process component */
    UPROPERTY()
    UPostProcessComponent* PostProcessComponent;

    /** Hit feedback material */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    UMaterialInterface* HitFeedbackMaterial;

    /** Hit feedback material instance */
    UPROPERTY()
    UMaterialInstanceDynamic* HitFeedbackMID;

    /** Low health material */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    UMaterialInterface* LowHealthMaterial;

    /** Low health material instance */
    UPROPERTY()
    UMaterialInstanceDynamic* LowHealthMID;

    /** Death material */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    UMaterialInterface* DeathMaterial;

    /** Death material instance */
    UPROPERTY()
    UMaterialInstanceDynamic* DeathMID;

    /** Hit feedback intensity */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    float HitFeedbackIntensity;

    /** Hit feedback duration */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    float HitFeedbackDuration;

    /** Low health threshold */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Post Process")
    float LowHealthThreshold;

    /** Current hit feedback value */
    float CurrentHitFeedback;

    /** Current low health value */
    float CurrentLowHealth;

    /** Current death value */
    float CurrentDeath;

    /** Timer handle for hit feedback */
    FTimerHandle TimerHandle_HitFeedback;

    /** Timer handle for death */
    FTimerHandle TimerHandle_Death;

    /** Reset hit feedback */
    UFUNCTION()
    void ResetHitFeedback();

    /** Update hit feedback */
    void UpdateHitFeedback(float DeltaTime);

    /** Update low health effect */
    void UpdateLowHealthEffect(float HealthPercent);

    /** Update death effect */
    void UpdateDeathEffect(float DeltaTime);

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Show hit feedback */
    UFUNCTION(BlueprintCallable, Category = "Post Process")
    void ShowHitFeedback(float Intensity = 1.0f);

    /** Show low health effect */
    UFUNCTION(BlueprintCallable, Category = "Post Process")
    void ShowLowHealthEffect(float HealthPercent);

    /** Show death effect */
    UFUNCTION(BlueprintCallable, Category = "Post Process")
    void ShowDeathEffect();
};
