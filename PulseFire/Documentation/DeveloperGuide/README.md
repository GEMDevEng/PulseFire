# PulseFire Developer Guide

This guide provides comprehensive information for developers working on the PulseFire project. It covers the architecture, key systems, and best practices for contributing to the codebase.

## Table of Contents

1. [Project Architecture](#project-architecture)
2. [Core Systems](#core-systems)
3. [Gameplay Systems](#gameplay-systems)
4. [Multiplayer Implementation](#multiplayer-implementation)
5. [Performance Optimization](#performance-optimization)
6. [Audio and Visual Effects](#audio-and-visual-effects)
7. [User Interface](#user-interface)
8. [Build and Deployment](#build-and-deployment)
9. [Coding Standards](#coding-standards)
10. [Contributing Guidelines](#contributing-guidelines)

## Project Architecture

### Overview

PulseFire is built using Unreal Engine's component-based architecture. The project is organized into several key modules:

- **Core Game Framework**: Base classes and systems that define the game's foundation
- **Character System**: Player character implementation with movement and input handling
- **Weapon System**: Weapon classes, firing mechanics, and projectile handling
- **Multiplayer Framework**: Networking, replication, and session management
- **UI System**: HUD, menus, and player feedback elements
- **Performance Tools**: Profiling and optimization utilities

### Directory Structure

```
PulseFire/
├── Config/                 # Engine and game configuration
├── Content/                # Game assets
│   ├── Animations/         # Character and weapon animations
│   ├── Blueprints/         # Blueprint assets
│   ├── Maps/               # Game levels
│   ├── Materials/          # Material assets
│   ├── Meshes/             # 3D models
│   ├── Sounds/             # Audio assets
│   └── Textures/           # Texture assets
├── Source/                 # C++ source code
│   └── PulseFire/          # Main game code
│       ├── Components/     # Game components
│       ├── Weapons/        # Weapon system
│       ├── Multiplayer/    # Multiplayer functionality
│       ├── UI/             # User interface
│       ├── Performance/    # Performance tools
│       ├── Audio/          # Audio system
│       └── VFX/            # Visual effects
└── Documentation/          # Project documentation
```

## Core Systems

### Game Instance

The `UPulseFireGameInstance` class manages the game's lifecycle, including:

- Session management
- Player persistence
- Game state transitions
- Settings management

```cpp
// Example: Creating a game session
void UPulseFireGameInstance::CreateSession(int32 NumPublicConnections)
{
    FOnlineSessionSettings SessionSettings;
    SessionSettings.NumPublicConnections = NumPublicConnections;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}
```

### Health Component

The `UHealthComponent` handles character health, damage, and death:

- Health tracking
- Damage application
- Death handling
- Team damage rules

```cpp
// Example: Applying damage
float UHealthComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
        return 0.0f;
        
    // Apply damage
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
    
    // Broadcast health changed event
    OnHealthChanged.Broadcast(this, CurrentHealth, -DamageAmount, DamageEvent.DamageTypeClass, EventInstigator, DamageCauser);
    
    // Check for death
    if (CurrentHealth <= 0.0f)
    {
        bIsDead = true;
        OnDeath.Broadcast(this, EventInstigator, DamageCauser);
    }
    
    return DamageAmount;
}
```

### Movement Component

The `UPulseFireMovementComponent` extends the character movement component with:

- Sprint functionality
- Slide mechanics
- Mantling
- Aim movement adjustments

```cpp
// Example: Sprint implementation
void UPulseFireMovementComponent::StartSprint()
{
    if (!CanSprint())
        return;
        
    bWantsToSprint = true;
    MaxWalkSpeed = SprintSpeed;
}

void UPulseFireMovementComponent::StopSprint()
{
    bWantsToSprint = false;
    MaxWalkSpeed = bIsAiming ? AimWalkSpeed : WalkSpeed;
}
```

## Gameplay Systems

### Weapon System

The weapon system is built around the `ABaseWeapon` class, which provides:

- Firing mechanics
- Ammo management
- Reloading
- Weapon switching
- Attachments

#### Weapon Types

- `AAssaultRifle`: Balanced, automatic weapon
- `AShotgun`: Close-range, high-damage weapon
- `ASniperRifle`: Long-range, precision weapon

```cpp
// Example: Weapon firing
void ABaseWeapon::Fire()
{
    if (!CanFire())
        return;
        
    // Consume ammo
    CurrentAmmo--;
    
    // Calculate spread
    float CurrentSpread = bIsAiming ? AimSpread : HipFireSpread;
    
    // Apply recoil
    ApplyRecoil();
    
    // Perform trace
    FHitResult HitResult;
    if (PerformWeaponTrace(HitResult))
    {
        // Apply damage
        ApplyDamage(HitResult);
        
        // Spawn impact effects
        PlayImpactEffects(HitResult);
    }
    
    // Play fire effects
    PlayFireEffects();
    
    // Start fire timer for next shot
    StartFireTimer();
}
```

### Pickup System

The pickup system is built around the `ABasePickup` class, which provides:

- Item collection
- Respawning
- Visual feedback
- Network replication

#### Pickup Types

- `AHealthPickup`: Restores player health
- `AAmmoPickup`: Replenishes weapon ammo
- `AWeaponPickup`: Provides a new weapon

```cpp
// Example: Pickup interaction
void ABasePickup::OnPickedUp(APulseFireCharacter* Character)
{
    if (!Character)
        return;
        
    // Apply pickup effect
    ApplyPickupEffect(Character);
    
    // Play pickup effects
    PlayPickupEffects();
    
    // Handle respawn
    if (bShouldRespawn)
    {
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ABasePickup::Respawn, RespawnTime);
    }
    else
    {
        Destroy();
    }
}
```

## Multiplayer Implementation

### Replication

PulseFire uses Unreal's replication system for network synchronization:

- Actor replication
- Property replication
- RPC (Remote Procedure Calls)
- Network relevancy

```cpp
// Example: Property replication
void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // Replicate to everyone
    DOREPLIFETIME(ABaseWeapon, CurrentAmmo);
    DOREPLIFETIME(ABaseWeapon, CurrentReserveAmmo);
    DOREPLIFETIME(ABaseWeapon, bIsReloading);
    
    // Replicate to owner only
    DOREPLIFETIME_CONDITION(ABaseWeapon, LastFireTime, COND_OwnerOnly);
}
```

### Game Modes

PulseFire includes several game modes:

- `APulseFireGameMode`: Base game mode with common functionality
- `ADeathMatchGameMode`: Free-for-all deathmatch
- `ATeamDeathMatchGameMode`: Team-based deathmatch
- `ACaptureTheFlagGameMode`: Objective-based flag capture

```cpp
// Example: Team assignment in CTF
int32 ACaptureTheFlagGameMode::AssignPlayerToTeam(APlayerController* PlayerController)
{
    // Count players on each team
    TArray<int32> PlayersPerTeam;
    CountPlayersPerTeam(PlayersPerTeam);
    
    // Assign to team with fewer players
    return GetTeamWithFewerPlayers();
}
```

### Session Management

Session management is handled by the game instance:

- Session creation
- Session discovery
- Session joining
- Session destruction

## Performance Optimization

### Performance Profiler

The `UPerformanceProfiler` component provides real-time performance metrics:

- Frame rate monitoring
- Memory usage tracking
- Draw call counting
- Automatic optimization

```cpp
// Example: Auto-optimization
void UPerformanceProfiler::ApplyOptimizations()
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
    else
    {
        // Good frame rate, use high quality
        OptimizationLevel = 2; // High
    }
    
    // Apply scalability settings
    if (OptimizationLevel != PreviousOptimizationLevel && OptimizationLevel >= 0)
    {
        UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
        if (UserSettings)
        {
            UserSettings->SetOverallScalabilityLevel(OptimizationLevel);
            UserSettings->ApplySettings(false);
            PreviousOptimizationLevel = OptimizationLevel;
        }
    }
}
```

### LOD Manager

The `ALODManager` handles level of detail optimization:

- Distance-based LOD
- Screen-size-based LOD
- Performance-based LOD adjustment
- Particle system optimization

```cpp
// Example: LOD adjustment
void ALODManager::UpdateStaticMeshLOD(UStaticMeshComponent* MeshComponent, float Distance)
{
    if (!MeshComponent || !MeshComponent->GetStaticMesh())
        return;
        
    // Get appropriate LOD level
    ELODLevel LODLevel = GetLODLevelForDistance(Distance);
    
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
```

## Audio and Visual Effects

### Sound Manager

The `USoundManager` provides centralized audio control:

- Sound playback
- Volume management
- Spatial audio
- Sound categories

```cpp
// Example: Playing a sound
UAudioComponent* USoundManager::PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings, USceneComponent* AttachToComponent, FName AttachPointName, FVector LocationOffset, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed)
{
    if (!Sound)
        return nullptr;
        
    // Apply SFX volume
    VolumeMultiplier *= SFXVolume;
    
    // Play sound
    return UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, Sound, Location, FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings);
}
```

### VFX Manager

The `UVFXManager` handles visual effects:

- Particle effects
- Decals
- Impact effects
- Muzzle flashes

```cpp
// Example: Spawning an impact effect
UParticleSystemComponent* UVFXManager::SpawnImpactEffect(const UObject* WorldContextObject, UParticleSystem* ImpactTemplate, const FVector& Location, const FVector& Normal)
{
    if (!ImpactTemplate)
        return nullptr;
        
    // Calculate rotation from normal
    FRotator Rotation = Normal.Rotation();
    
    // Spawn impact effect
    UParticleSystemComponent* PSC = SpawnEmitterAtLocation(WorldContextObject, ImpactTemplate, Location, Rotation);
    
    // Set auto destroy
    if (PSC)
    {
        PSC->SetAutoDestroy(true);
    }
    
    return PSC;
}
```

### Post-Processing Manager

The `UPostProcessManager` handles screen effects:

- Hit feedback
- Low health effects
- Death effects
- Environmental effects

## User Interface

### Base User Widget

The `UPulseFireUserWidget` provides common functionality for all UI elements:

- Animation handling
- Input processing
- Navigation
- Visibility control

### HUD Widget

The `UHUDWidget` displays in-game information:

- Health bar
- Ammo counter
- Crosshair
- Score display
- Timer

### Menu System

The menu system includes:

- Main menu
- Multiplayer menu
- Settings menu
- Pause menu
- Results screen

## Build and Deployment

### Development Build

To create a development build:

1. Open the project in Unreal Editor
2. Select **File > Package Project > Development**
3. Choose the target platform
4. Select the output directory
5. Click **Package**

### Release Build

To create a release build:

1. Open the project in Unreal Editor
2. Select **File > Package Project > Shipping**
3. Choose the target platform
4. Select the output directory
5. Click **Package**

### Continuous Integration

PulseFire uses GitHub Actions for CI/CD:

- Automated builds
- Unit testing
- Static code analysis
- Deployment to test environments

## Coding Standards

### Naming Conventions

- **Classes**: `APrefixClassName`
- **Interfaces**: `IPrefixInterfaceName`
- **Variables**: `CamelCase` with appropriate prefix
- **Functions**: `PascalCase`
- **Enums**: `EPrefixEnumName`
- **Structs**: `FPrefixStructName`

### Code Organization

- One class per file
- Group related functionality
- Use regions for clarity
- Keep functions focused and small

### Comments

- Use Doxygen-style comments for public API
- Comment complex algorithms
- Explain non-obvious design decisions

```cpp
/**
 * Applies damage to the character.
 * 
 * @param DamageAmount The amount of damage to apply
 * @param DamageEvent The damage event containing additional information
 * @param EventInstigator The controller that caused the damage
 * @param DamageCauser The actor that caused the damage
 * @return The amount of damage actually applied
 */
float UHealthComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // Implementation...
}
```

## Contributing Guidelines

### Workflow

1. Create a feature branch from `develop`
2. Implement your changes
3. Write unit tests
4. Submit a pull request
5. Address review feedback
6. Merge to `develop`

### Pull Request Requirements

- Code must compile without warnings
- Unit tests must pass
- Code must follow project standards
- Documentation must be updated
- Performance impact must be considered

### Issue Tracking

Use GitHub Issues for bug reports and feature requests:

- **Bug Reports**: Include steps to reproduce, expected vs. actual behavior, and system information
- **Feature Requests**: Include use case, acceptance criteria, and priority
