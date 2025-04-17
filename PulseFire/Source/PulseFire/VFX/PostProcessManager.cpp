#include "PostProcessManager.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UPostProcessManager::UPostProcessManager()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame
    PrimaryComponentTick.bCanEverTick = true;

    // Set default values
    HitFeedbackIntensity = 1.0f;
    HitFeedbackDuration = 0.5f;
    LowHealthThreshold = 0.3f;
    CurrentHitFeedback = 0.0f;
    CurrentLowHealth = 0.0f;
    CurrentDeath = 0.0f;
}

// Called when the game starts
void UPostProcessManager::BeginPlay()
{
    Super::BeginPlay();

    // Create post process component
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character)
    {
        PostProcessComponent = NewObject<UPostProcessComponent>(Character, TEXT("PostProcessComponent"));
        PostProcessComponent->RegisterComponent();
        PostProcessComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        PostProcessComponent->bUnbound = true;
        PostProcessComponent->bEnabled = true;
        PostProcessComponent->Priority = 1.0f;
        PostProcessComponent->BlendWeight = 1.0f;

        // Create material instances
        if (HitFeedbackMaterial)
        {
            HitFeedbackMID = UMaterialInstanceDynamic::Create(HitFeedbackMaterial, this);
            if (HitFeedbackMID)
            {
                HitFeedbackMID->SetScalarParameterValue("Intensity", 0.0f);
                PostProcessComponent->Settings.AddBlendable(HitFeedbackMID, 1.0f);
            }
        }

        if (LowHealthMaterial)
        {
            LowHealthMID = UMaterialInstanceDynamic::Create(LowHealthMaterial, this);
            if (LowHealthMID)
            {
                LowHealthMID->SetScalarParameterValue("Intensity", 0.0f);
                PostProcessComponent->Settings.AddBlendable(LowHealthMID, 1.0f);
            }
        }

        if (DeathMaterial)
        {
            DeathMID = UMaterialInstanceDynamic::Create(DeathMaterial, this);
            if (DeathMID)
            {
                DeathMID->SetScalarParameterValue("Intensity", 0.0f);
                PostProcessComponent->Settings.AddBlendable(DeathMID, 1.0f);
            }
        }
    }
}

// Called every frame
void UPostProcessManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update effects
    UpdateHitFeedback(DeltaTime);
    UpdateDeathEffect(DeltaTime);
}

void UPostProcessManager::ShowHitFeedback(float Intensity)
{
    // Set hit feedback intensity
    CurrentHitFeedback = FMath::Max(CurrentHitFeedback, Intensity * HitFeedbackIntensity);

    // Update material
    if (HitFeedbackMID)
    {
        HitFeedbackMID->SetScalarParameterValue("Intensity", CurrentHitFeedback);
    }

    // Set timer to reset hit feedback
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_HitFeedback, this, &UPostProcessManager::ResetHitFeedback, HitFeedbackDuration, false);
}

void UPostProcessManager::ShowLowHealthEffect(float HealthPercent)
{
    // Update low health effect
    UpdateLowHealthEffect(HealthPercent);
}

void UPostProcessManager::ShowDeathEffect()
{
    // Set death effect
    CurrentDeath = 1.0f;

    // Update material
    if (DeathMID)
    {
        DeathMID->SetScalarParameterValue("Intensity", CurrentDeath);
    }

    // Set timer to fade in death effect
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Death, this, &UPostProcessManager::ResetHitFeedback, 0.1f, false);
}

void UPostProcessManager::ResetHitFeedback()
{
    // Reset hit feedback
    CurrentHitFeedback = 0.0f;

    // Update material
    if (HitFeedbackMID)
    {
        HitFeedbackMID->SetScalarParameterValue("Intensity", CurrentHitFeedback);
    }
}

void UPostProcessManager::UpdateHitFeedback(float DeltaTime)
{
    // Gradually reduce hit feedback
    if (CurrentHitFeedback > 0.0f)
    {
        CurrentHitFeedback = FMath::Max(0.0f, CurrentHitFeedback - (DeltaTime / HitFeedbackDuration));

        // Update material
        if (HitFeedbackMID)
        {
            HitFeedbackMID->SetScalarParameterValue("Intensity", CurrentHitFeedback);
        }
    }
}

void UPostProcessManager::UpdateLowHealthEffect(float HealthPercent)
{
    // Calculate low health intensity
    float TargetIntensity = 0.0f;
    if (HealthPercent < LowHealthThreshold)
    {
        TargetIntensity = 1.0f - (HealthPercent / LowHealthThreshold);
    }

    // Update current value
    CurrentLowHealth = TargetIntensity;

    // Update material
    if (LowHealthMID)
    {
        LowHealthMID->SetScalarParameterValue("Intensity", CurrentLowHealth);
    }
}

void UPostProcessManager::UpdateDeathEffect(float DeltaTime)
{
    // Gradually increase death effect
    if (CurrentDeath > 0.0f && CurrentDeath < 1.0f)
    {
        CurrentDeath = FMath::Min(1.0f, CurrentDeath + (DeltaTime * 0.5f));

        // Update material
        if (DeathMID)
        {
            DeathMID->SetScalarParameterValue("Intensity", CurrentDeath);
        }
    }
}
