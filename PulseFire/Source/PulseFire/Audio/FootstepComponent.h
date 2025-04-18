#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FootstepComponent.generated.h"

class USoundBase;

USTRUCT(BlueprintType)
struct FFootstepSounds
{
    GENERATED_BODY()

    /** Sound for default surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* DefaultSound;

    /** Sound for concrete surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* ConcreteSound;

    /** Sound for dirt surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* DirtSound;

    /** Sound for metal surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* MetalSound;

    /** Sound for wood surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* WoodSound;

    /** Sound for water surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* WaterSound;

    /** Sound for grass surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* GrassSound;

    /** Sound for glass surface */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    USoundBase* GlassSound;
};

/**
 * Component for handling character footstep sounds.
 */
UCLASS(ClassGroup=(Audio), meta=(BlueprintSpawnableComponent))
class PULSEFIRE_API UFootstepComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UFootstepComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    /** Footstep sounds for walking */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    FFootstepSounds WalkSounds;

    /** Footstep sounds for running */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    FFootstepSounds RunSounds;

    /** Footstep sounds for crouching */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    FFootstepSounds CrouchSounds;

    /** Volume multiplier for footsteps */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    float VolumeMultiplier;

    /** Pitch variation for footsteps */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    float PitchVariation;

    /** Minimum speed to play footstep sounds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    float MinSpeedToPlaySound;

    /** Trace distance for surface detection */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
    float TraceDistance;

    /** Timer handle for footstep sounds */
    FTimerHandle TimerHandle_Footstep;

    /** Last time a footstep was played */
    float LastFootstepTime;

    /** Check for footstep sounds */
    void CheckForFootstep();

    /** Get the appropriate footstep sound based on surface type and movement state */
    USoundBase* GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType, bool bIsRunning, bool bIsCrouching);

    /** Get the surface type at the character's feet */
    TEnumAsByte<EPhysicalSurface> GetSurfaceType();

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Play a footstep sound */
    UFUNCTION(BlueprintCallable, Category = "Footstep")
    void PlayFootstepSound();
};
