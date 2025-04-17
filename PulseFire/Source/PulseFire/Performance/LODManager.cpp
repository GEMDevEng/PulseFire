#include "LODManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values
ALODManager::ALODManager()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    LODSettings = FLODSettings();
    UpdateInterval = 1.0f;
    bAutoAdjustLOD = false;
    TargetFrameRate = 60.0f;
    CurrentFrameRate = 0.0f;
    AverageFrameRate = 0.0f;
    FrameRateSamples = 0;
}

// Called when the game starts or when spawned
void ALODManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Start updating LOD
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateLOD, this, &ALODManager::UpdateLOD, UpdateInterval, true);
}

// Called every frame
void ALODManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update frame rate
    CurrentFrameRate = 1.0f / DeltaTime;

    // Update running average of frame rate
    AverageFrameRate = (AverageFrameRate * FrameRateSamples + CurrentFrameRate) / (FrameRateSamples + 1);
    FrameRateSamples++;

    // Adjust LOD based on performance every 60 frames
    if (bAutoAdjustLOD && FrameRateSamples >= 60)
    {
        AdjustLODForPerformance();
        AverageFrameRate = 0.0f;
        FrameRateSamples = 0;
    }
}

void ALODManager::UpdateLOD()
{
    // Get player camera location
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !PC->PlayerCameraManager)
    {
        return;
    }

    FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

    // Update LOD for all static mesh components
    TArray<UStaticMeshComponent*> StaticMeshComponents;
    GetObjectsOfClass<UStaticMeshComponent>(StaticMeshComponents);
    for (UStaticMeshComponent* MeshComponent : StaticMeshComponents)
    {
        if (MeshComponent && !MeshComponent->IsPendingKill() && MeshComponent->IsVisible())
        {
            // Calculate distance to camera
            float Distance = FVector::Dist(MeshComponent->GetComponentLocation(), CameraLocation);
            
            // Update LOD
            UpdateStaticMeshLOD(MeshComponent, Distance);
        }
    }

    // Update LOD for all particle system components
    TArray<UParticleSystemComponent*> ParticleSystems;
    GetObjectsOfClass<UParticleSystemComponent>(ParticleSystems);
    for (UParticleSystemComponent* ParticleSystem : ParticleSystems)
    {
        if (ParticleSystem && !ParticleSystem->IsPendingKill() && ParticleSystem->IsActive())
        {
            // Calculate distance to camera
            float Distance = FVector::Dist(ParticleSystem->GetComponentLocation(), CameraLocation);
            
            // Update LOD
            UpdateParticleSystemLOD(ParticleSystem, Distance);
        }
    }
}

void ALODManager::UpdateStaticMeshLOD(UStaticMeshComponent* MeshComponent, float Distance)
{
    if (!MeshComponent || !MeshComponent->GetStaticMesh())
    {
        return;
    }

    // Get appropriate LOD level
    ELODLevel LODLevel;
    if (LODSettings.bUseScreenSize)
    {
        // Calculate screen size
        float BoundsRadius = MeshComponent->Bounds.SphereRadius;
        float ScreenSize = BoundsRadius / FMath::Max(1.0f, Distance);
        
        // Get LOD level based on screen size
        LODLevel = GetLODLevelForScreenSize(ScreenSize);
    }
    else
    {
        // Get LOD level based on distance
        LODLevel = GetLODLevelForDistance(Distance);
    }

    // Apply LOD level
    int32 ForcedLOD = -1; // Auto LOD
    switch (LODLevel)
    {
    case ELODLevel::High:
        ForcedLOD = 0;
        break;
    case ELODLevel::Medium:
        ForcedLOD = 1;
        break;
    case ELODLevel::Low:
        ForcedLOD = 2;
        break;
    case ELODLevel::Off:
        MeshComponent->SetVisibility(false);
        return;
    }

    // Apply forced LOD if the mesh has enough LOD levels
    if (MeshComponent->GetStaticMesh()->GetNumLODs() > ForcedLOD)
    {
        MeshComponent->SetVisibility(true);
        MeshComponent->SetForcedLodModel(ForcedLOD + 1); // +1 because 0 means auto
    }
}

void ALODManager::UpdateParticleSystemLOD(UParticleSystemComponent* ParticleSystem, float Distance)
{
    if (!ParticleSystem)
    {
        return;
    }

    // Get appropriate LOD level
    ELODLevel LODLevel;
    if (LODSettings.bUseScreenSize)
    {
        // Calculate screen size
        float BoundsRadius = ParticleSystem->Bounds.SphereRadius;
        float ScreenSize = BoundsRadius / FMath::Max(1.0f, Distance);
        
        // Get LOD level based on screen size
        LODLevel = GetLODLevelForScreenSize(ScreenSize);
    }
    else
    {
        // Get LOD level based on distance
        LODLevel = GetLODLevelForDistance(Distance);
    }

    // Apply LOD level
    switch (LODLevel)
    {
    case ELODLevel::High:
        ParticleSystem->SetDetailMode(DM_High);
        break;
    case ELODLevel::Medium:
        ParticleSystem->SetDetailMode(DM_Medium);
        break;
    case ELODLevel::Low:
        ParticleSystem->SetDetailMode(DM_Low);
        break;
    case ELODLevel::Off:
        ParticleSystem->SetActive(false);
        break;
    }
}

ELODLevel ALODManager::GetLODLevelForDistance(float Distance) const
{
    if (Distance <= LODSettings.HighLODDistance)
    {
        return ELODLevel::High;
    }
    else if (Distance <= LODSettings.MediumLODDistance)
    {
        return ELODLevel::Medium;
    }
    else if (Distance <= LODSettings.LowLODDistance)
    {
        return ELODLevel::Low;
    }
    else if (Distance <= LODSettings.MaxLODDistance)
    {
        return ELODLevel::Off;
    }
    else
    {
        return ELODLevel::Off;
    }
}

ELODLevel ALODManager::GetLODLevelForScreenSize(float ScreenSize) const
{
    if (ScreenSize >= LODSettings.HighLODScreenSize)
    {
        return ELODLevel::High;
    }
    else if (ScreenSize >= LODSettings.MediumLODScreenSize)
    {
        return ELODLevel::Medium;
    }
    else if (ScreenSize >= LODSettings.LowLODScreenSize)
    {
        return ELODLevel::Low;
    }
    else
    {
        return ELODLevel::Off;
    }
}

void ALODManager::AdjustLODForPerformance()
{
    // Adjust LOD settings based on performance
    if (AverageFrameRate < TargetFrameRate * 0.8f)
    {
        // Frame rate is too low, reduce LOD quality
        if (LODSettings.LODLevel == ELODLevel::High)
        {
            LODSettings.LODLevel = ELODLevel::Medium;
            OnLODChanged.Broadcast(LODSettings.LODLevel);
        }
        else if (LODSettings.LODLevel == ELODLevel::Medium)
        {
            LODSettings.LODLevel = ELODLevel::Low;
            OnLODChanged.Broadcast(LODSettings.LODLevel);
        }
    }
    else if (AverageFrameRate > TargetFrameRate * 1.2f)
    {
        // Frame rate is high, increase LOD quality
        if (LODSettings.LODLevel == ELODLevel::Low)
        {
            LODSettings.LODLevel = ELODLevel::Medium;
            OnLODChanged.Broadcast(LODSettings.LODLevel);
        }
        else if (LODSettings.LODLevel == ELODLevel::Medium)
        {
            LODSettings.LODLevel = ELODLevel::High;
            OnLODChanged.Broadcast(LODSettings.LODLevel);
        }
    }

    // Adjust LOD distances based on LOD level
    switch (LODSettings.LODLevel)
    {
    case ELODLevel::High:
        LODSettings.HighLODDistance = 1000.0f;
        LODSettings.MediumLODDistance = 2000.0f;
        LODSettings.LowLODDistance = 4000.0f;
        LODSettings.MaxLODDistance = 8000.0f;
        break;
    case ELODLevel::Medium:
        LODSettings.HighLODDistance = 500.0f;
        LODSettings.MediumLODDistance = 1000.0f;
        LODSettings.LowLODDistance = 2000.0f;
        LODSettings.MaxLODDistance = 4000.0f;
        break;
    case ELODLevel::Low:
        LODSettings.HighLODDistance = 250.0f;
        LODSettings.MediumLODDistance = 500.0f;
        LODSettings.LowLODDistance = 1000.0f;
        LODSettings.MaxLODDistance = 2000.0f;
        break;
    default:
        break;
    }
}

FLODSettings ALODManager::GetLODSettings() const
{
    return LODSettings;
}

void ALODManager::SetLODSettings(const FLODSettings& NewSettings)
{
    // Check if LOD level has changed
    if (LODSettings.LODLevel != NewSettings.LODLevel)
    {
        OnLODChanged.Broadcast(NewSettings.LODLevel);
    }

    LODSettings = NewSettings;
}

void ALODManager::SetUpdateInterval(float NewInterval)
{
    UpdateInterval = FMath::Max(0.1f, NewInterval);

    // Update timer
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateLOD);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateLOD, this, &ALODManager::UpdateLOD, UpdateInterval, true);
}

void ALODManager::EnableAutoAdjustLOD(bool bEnabled)
{
    bAutoAdjustLOD = bEnabled;

    // Reset performance variables
    if (bEnabled)
    {
        AverageFrameRate = 0.0f;
        FrameRateSamples = 0;
    }
}

void ALODManager::SetTargetFrameRate(float NewFrameRate)
{
    TargetFrameRate = FMath::Max(30.0f, NewFrameRate);
}

ALODManager* ALODManager::GetInstance(UObject* WorldContextObject)
{
    // Find the LOD manager in the world
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return nullptr;
    }

    // Find the first LOD manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, ALODManager::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        return Cast<ALODManager>(FoundActors[0]);
    }

    // Spawn a new LOD manager if none exists
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<ALODManager>(ALODManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}
