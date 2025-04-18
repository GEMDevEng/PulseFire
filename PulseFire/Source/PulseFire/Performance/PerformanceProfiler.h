#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerformanceProfiler.generated.h"

USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    /** Frame rate (frames per second) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float FrameRate;

    /** Frame time (milliseconds) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float FrameTime;

    /** Game thread time (milliseconds) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float GameThreadTime;

    /** Render thread time (milliseconds) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float RenderThreadTime;

    /** GPU time (milliseconds) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float GPUTime;

    /** Memory usage (megabytes) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MemoryUsage;

    /** Ping (milliseconds) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float Ping;

    /** Packet loss (percentage) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float PacketLoss;

    /** Number of actors in the level */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 ActorCount;

    /** Number of draw calls */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 DrawCalls;

    /** Number of triangles (thousands) */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float TriangleCount;

    /** Number of particles */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 ParticleCount;

    FPerformanceMetrics()
    {
        FrameRate = 0.0f;
        FrameTime = 0.0f;
        GameThreadTime = 0.0f;
        RenderThreadTime = 0.0f;
        GPUTime = 0.0f;
        MemoryUsage = 0.0f;
        Ping = 0.0f;
        PacketLoss = 0.0f;
        ActorCount = 0;
        DrawCalls = 0;
        TriangleCount = 0.0f;
        ParticleCount = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceMetricsUpdated, const FPerformanceMetrics&, Metrics);

/**
 * Component for profiling and optimizing game performance.
 */
UCLASS(ClassGroup=(Performance), meta=(BlueprintSpawnableComponent))
class PULSEFIRE_API UPerformanceProfiler : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UPerformanceProfiler();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    /** Update interval in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Performance")
    float UpdateInterval;

    /** Whether to enable detailed profiling */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Performance")
    bool bDetailedProfiling;

    /** Whether to show debug overlay */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Performance")
    bool bShowDebugOverlay;

    /** Current performance metrics */
    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    FPerformanceMetrics CurrentMetrics;

    /** Timer handle for updating metrics */
    FTimerHandle TimerHandle_UpdateMetrics;

    /** Update performance metrics */
    UFUNCTION()
    void UpdateMetrics();

    /** Get frame rate and frame time */
    void GetFrameMetrics();

    /** Get thread times */
    void GetThreadTimes();

    /** Get memory usage */
    void GetMemoryUsage();

    /** Get network metrics */
    void GetNetworkMetrics();

    /** Get scene metrics */
    void GetSceneMetrics();

    /** Get rendering metrics */
    void GetRenderingMetrics();

    /** Draw debug overlay */
    void DrawDebugOverlay();

    /** Apply performance optimizations based on metrics */
    void ApplyOptimizations();

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Get current performance metrics */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    FPerformanceMetrics GetPerformanceMetrics() const;

    /** Set update interval */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetUpdateInterval(float NewInterval);

    /** Set detailed profiling */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetDetailedProfiling(bool bEnabled);

    /** Set debug overlay */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetDebugOverlay(bool bEnabled);

    /** Enable automatic performance optimization */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void EnableAutoOptimization(bool bEnabled);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Performance")
    FOnPerformanceMetricsUpdated OnPerformanceMetricsUpdated;

private:
    /** Whether to enable automatic performance optimization */
    bool bAutoOptimizationEnabled;

    /** Running average of frame rate */
    float AverageFrameRate;

    /** Number of samples for running average */
    int32 FrameRateSamples;

    /** Previous optimization level */
    int32 PreviousOptimizationLevel;
};
