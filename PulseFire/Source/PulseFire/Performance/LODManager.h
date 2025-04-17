#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LODManager.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

UENUM(BlueprintType)
enum class ELODLevel : uint8
{
    High    UMETA(DisplayName = "High"),
    Medium  UMETA(DisplayName = "Medium"),
    Low     UMETA(DisplayName = "Low"),
    Off     UMETA(DisplayName = "Off")
};

USTRUCT(BlueprintType)
struct FLODSettings
{
    GENERATED_BODY()

    /** Current LOD level */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    ELODLevel LODLevel;

    /** Distance for high LOD (meters) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float HighLODDistance;

    /** Distance for medium LOD (meters) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float MediumLODDistance;

    /** Distance for low LOD (meters) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float LowLODDistance;

    /** Maximum distance for any LOD (meters) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    float MaxLODDistance;

    /** Whether to use screen size instead of distance */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    bool bUseScreenSize;

    /** Screen size for high LOD (0.0-1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD", meta = (EditCondition = "bUseScreenSize"))
    float HighLODScreenSize;

    /** Screen size for medium LOD (0.0-1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD", meta = (EditCondition = "bUseScreenSize"))
    float MediumLODScreenSize;

    /** Screen size for low LOD (0.0-1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD", meta = (EditCondition = "bUseScreenSize"))
    float LowLODScreenSize;

    FLODSettings()
    {
        LODLevel = ELODLevel::High;
        HighLODDistance = 1000.0f;
        MediumLODDistance = 2000.0f;
        LowLODDistance = 4000.0f;
        MaxLODDistance = 8000.0f;
        bUseScreenSize = false;
        HighLODScreenSize = 0.5f;
        MediumLODScreenSize = 0.25f;
        LowLODScreenSize = 0.1f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLODChanged, ELODLevel, NewLODLevel);

/**
 * Manager for handling level of detail (LOD) optimization.
 */
UCLASS()
class PULSEFIRE_API ALODManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ALODManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** LOD settings */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LOD")
    FLODSettings LODSettings;

    /** Update interval in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LOD")
    float UpdateInterval;

    /** Whether to enable automatic LOD adjustment based on performance */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LOD")
    bool bAutoAdjustLOD;

    /** Target frame rate for auto-adjustment */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LOD", meta = (EditCondition = "bAutoAdjustLOD"))
    float TargetFrameRate;

    /** Timer handle for updating LOD */
    FTimerHandle TimerHandle_UpdateLOD;

    /** Update LOD for all relevant actors */
    UFUNCTION()
    void UpdateLOD();

    /** Update LOD for a specific static mesh component */
    void UpdateStaticMeshLOD(UStaticMeshComponent* MeshComponent, float Distance);

    /** Update LOD for a specific particle system component */
    void UpdateParticleSystemLOD(UParticleSystemComponent* ParticleSystem, float Distance);

    /** Get appropriate LOD level based on distance */
    ELODLevel GetLODLevelForDistance(float Distance) const;

    /** Get appropriate LOD level based on screen size */
    ELODLevel GetLODLevelForScreenSize(float ScreenSize) const;

    /** Adjust LOD settings based on performance */
    void AdjustLODForPerformance();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Get current LOD settings */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    FLODSettings GetLODSettings() const;

    /** Set LOD settings */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetLODSettings(const FLODSettings& NewSettings);

    /** Set update interval */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetUpdateInterval(float NewInterval);

    /** Enable automatic LOD adjustment */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void EnableAutoAdjustLOD(bool bEnabled);

    /** Set target frame rate for auto-adjustment */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetTargetFrameRate(float NewFrameRate);

    /** Get the singleton instance */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    static ALODManager* GetInstance(UObject* WorldContextObject);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "LOD")
    FOnLODChanged OnLODChanged;

private:
    /** Current frame rate */
    float CurrentFrameRate;

    /** Running average of frame rate */
    float AverageFrameRate;

    /** Number of samples for running average */
    int32 FrameRateSamples;
};
