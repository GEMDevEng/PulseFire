#include "PerformanceProfiler.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPerformanceProfiler::UPerformanceProfiler()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame
    PrimaryComponentTick.bCanEverTick = true;

    // Set default values
    UpdateInterval = 1.0f;
    bDetailedProfiling = false;
    bShowDebugOverlay = false;
    bAutoOptimizationEnabled = false;
    AverageFrameRate = 0.0f;
    FrameRateSamples = 0;
    PreviousOptimizationLevel = -1;
}

// Called when the game starts
void UPerformanceProfiler::BeginPlay()
{
    Super::BeginPlay();
    
    // Start updating metrics
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateMetrics, this, &UPerformanceProfiler::UpdateMetrics, UpdateInterval, true);
}

// Called every frame
void UPerformanceProfiler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update frame metrics every frame
    GetFrameMetrics();

    // Draw debug overlay if enabled
    if (bShowDebugOverlay)
    {
        DrawDebugOverlay();
    }

    // Apply optimizations if auto-optimization is enabled
    if (bAutoOptimizationEnabled)
    {
        // Update running average of frame rate
        AverageFrameRate = (AverageFrameRate * FrameRateSamples + CurrentMetrics.FrameRate) / (FrameRateSamples + 1);
        FrameRateSamples++;

        // Apply optimizations every 60 frames
        if (FrameRateSamples >= 60)
        {
            ApplyOptimizations();
            AverageFrameRate = 0.0f;
            FrameRateSamples = 0;
        }
    }
}

void UPerformanceProfiler::UpdateMetrics()
{
    // Get thread times
    if (bDetailedProfiling)
    {
        GetThreadTimes();
    }

    // Get memory usage
    GetMemoryUsage();

    // Get network metrics
    GetNetworkMetrics();

    // Get scene metrics
    GetSceneMetrics();

    // Get rendering metrics
    if (bDetailedProfiling)
    {
        GetRenderingMetrics();
    }

    // Broadcast metrics updated event
    OnPerformanceMetricsUpdated.Broadcast(CurrentMetrics);
}

void UPerformanceProfiler::GetFrameMetrics()
{
    // Get frame rate and frame time
    if (GEngine)
    {
        CurrentMetrics.FrameRate = 1.0f / GetWorld()->GetDeltaSeconds();
        CurrentMetrics.FrameTime = GetWorld()->GetDeltaSeconds() * 1000.0f; // Convert to milliseconds
    }
}

void UPerformanceProfiler::GetThreadTimes()
{
    // Get thread times
    if (GEngine)
    {
        // These values are not directly accessible in the public API
        // In a real implementation, you would use engine stats or profiling tools
        // For now, we'll use placeholder values
        CurrentMetrics.GameThreadTime = 0.0f;
        CurrentMetrics.RenderThreadTime = 0.0f;
        CurrentMetrics.GPUTime = 0.0f;
    }
}

void UPerformanceProfiler::GetMemoryUsage()
{
    // Get memory usage
    if (GEngine)
    {
        FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
        CurrentMetrics.MemoryUsage = MemStats.UsedPhysical / (1024.0f * 1024.0f); // Convert to MB
    }
}

void UPerformanceProfiler::GetNetworkMetrics()
{
    // Get network metrics
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        CurrentMetrics.Ping = PC->PlayerState ? PC->PlayerState->GetPingInMilliseconds() : 0.0f;
        
        // Packet loss is not directly accessible in the public API
        // In a real implementation, you would use engine stats or networking tools
        CurrentMetrics.PacketLoss = 0.0f;
    }
}

void UPerformanceProfiler::GetSceneMetrics()
{
    // Get scene metrics
    UWorld* World = GetWorld();
    if (World)
    {
        // Count actors
        CurrentMetrics.ActorCount = World->GetNumActors();

        // Count particles
        int32 ParticleCount = 0;
        for (TObjectIterator<UParticleSystemComponent> It; It; ++It)
        {
            if (It->GetWorld() == World && !It->IsPendingKill() && It->IsActive())
            {
                ParticleCount += It->GetNumActiveParticles();
            }
        }
        CurrentMetrics.ParticleCount = ParticleCount;
    }
}

void UPerformanceProfiler::GetRenderingMetrics()
{
    // Get rendering metrics
    // These values are not directly accessible in the public API
    // In a real implementation, you would use engine stats or rendering tools
    // For now, we'll use placeholder values
    CurrentMetrics.DrawCalls = 0;
    CurrentMetrics.TriangleCount = 0.0f;
}

void UPerformanceProfiler::DrawDebugOverlay()
{
    // Draw debug overlay
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        // Format metrics text
        FString MetricsText = FString::Printf(
            TEXT("FPS: %.1f (%.1f ms)\n")
            TEXT("Memory: %.1f MB\n")
            TEXT("Ping: %.1f ms\n")
            TEXT("Actors: %d\n")
            TEXT("Particles: %d"),
            CurrentMetrics.FrameRate,
            CurrentMetrics.FrameTime,
            CurrentMetrics.MemoryUsage,
            CurrentMetrics.Ping,
            CurrentMetrics.ActorCount,
            CurrentMetrics.ParticleCount
        );

        // Add detailed metrics if enabled
        if (bDetailedProfiling)
        {
            MetricsText += FString::Printf(
                TEXT("\nGame Thread: %.1f ms\n")
                TEXT("Render Thread: %.1f ms\n")
                TEXT("GPU: %.1f ms\n")
                TEXT("Draw Calls: %d\n")
                TEXT("Triangles: %.1f K"),
                CurrentMetrics.GameThreadTime,
                CurrentMetrics.RenderThreadTime,
                CurrentMetrics.GPUTime,
                CurrentMetrics.DrawCalls,
                CurrentMetrics.TriangleCount
            );
        }

        // Draw text on screen
        PC->ClientMessage(MetricsText);
    }
}

void UPerformanceProfiler::ApplyOptimizations()
{
    // Apply optimizations based on performance metrics
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (UserSettings)
    {
        // Determine optimization level based on frame rate
        int32 OptimizationLevel = -1;
        if (AverageFrameRate < 30.0f)
        {
            // Low frame rate, reduce quality
            OptimizationLevel = 0; // Low
        }
        else if (AverageFrameRate < 60.0f)
        {
            // Medium frame rate, use medium quality
            OptimizationLevel = 1; // Medium
        }
        else if (AverageFrameRate < 90.0f)
        {
            // Good frame rate, use high quality
            OptimizationLevel = 2; // High
        }
        else
        {
            // Excellent frame rate, use epic quality
            OptimizationLevel = 3; // Epic
        }

        // Only apply changes if optimization level has changed
        if (OptimizationLevel != PreviousOptimizationLevel && OptimizationLevel >= 0)
        {
            // Apply scalability settings
            UserSettings->SetOverallScalabilityLevel(OptimizationLevel);
            UserSettings->ApplySettings(false);

            // Update previous optimization level
            PreviousOptimizationLevel = OptimizationLevel;
        }
    }
}

FPerformanceMetrics UPerformanceProfiler::GetPerformanceMetrics() const
{
    return CurrentMetrics;
}

void UPerformanceProfiler::SetUpdateInterval(float NewInterval)
{
    UpdateInterval = FMath::Max(0.1f, NewInterval);

    // Update timer
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateMetrics);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateMetrics, this, &UPerformanceProfiler::UpdateMetrics, UpdateInterval, true);
}

void UPerformanceProfiler::SetDetailedProfiling(bool bEnabled)
{
    bDetailedProfiling = bEnabled;
}

void UPerformanceProfiler::SetDebugOverlay(bool bEnabled)
{
    bShowDebugOverlay = bEnabled;
}

void UPerformanceProfiler::EnableAutoOptimization(bool bEnabled)
{
    bAutoOptimizationEnabled = bEnabled;

    // Reset optimization variables
    if (bEnabled)
    {
        AverageFrameRate = 0.0f;
        FrameRateSamples = 0;
        PreviousOptimizationLevel = -1;
    }
}
