# PulseFire API Reference

This document provides a comprehensive reference for the PulseFire game's API, including classes, functions, and systems.

## Table of Contents

1. [Core Systems](#core-systems)
2. [Character System](#character-system)
3. [Weapon System](#weapon-system)
4. [Multiplayer System](#multiplayer-system)
5. [UI System](#ui-system)
6. [Audio System](#audio-system)
7. [Visual Effects System](#visual-effects-system)
8. [Performance System](#performance-system)

## Core Systems

### Game Instance

`UPulseFireGameInstance` - Manages the game's lifecycle and session handling.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `SessionInterface` | `IOnlineSessionPtr` | Interface for online session management |
| `SearchSettings` | `TSharedPtr<FOnlineSessionSearch>` | Settings for session search |
| `bIsLANMatch` | `bool` | Whether the session is a LAN match |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `CreateSession` | `void` | `int32 NumPublicConnections` | Creates a new game session |
| `FindSessions` | `void` | `int32 MaxSearchResults` | Searches for available sessions |
| `JoinSession` | `void` | `const FOnlineSessionSearchResult& SessionResult` | Joins an existing session |
| `DestroySession` | `void` | None | Destroys the current session |

### Health Component

`UHealthComponent` - Handles character health, damage, and death.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `MaxHealth` | `float` | Maximum health value |
| `CurrentHealth` | `float` | Current health value |
| `bIsDead` | `bool` | Whether the character is dead |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `TakeDamage` | `float` | `float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser` | Applies damage to the character |
| `Heal` | `void` | `float HealAmount` | Heals the character |
| `GetHealthPercent` | `float` | None | Returns health as a percentage |

#### Events

| Name | Parameters | Description |
|------|------------|-------------|
| `OnHealthChanged` | `UHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser` | Triggered when health changes |
| `OnDeath` | `UHealthComponent* HealthComp, AController* InstigatedBy, AActor* DamageCauser` | Triggered when the character dies |

## Character System

### PulseFire Character

`APulseFireCharacter` - The base character class for the game.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `HealthComponent` | `UHealthComponent*` | Health component reference |
| `MovementComponent` | `UPulseFireMovementComponent*` | Custom movement component |
| `CurrentWeapon` | `ABaseWeapon*` | Currently equipped weapon |
| `Inventory` | `TArray<ABaseWeapon*>` | Weapon inventory |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `Fire` | `void` | None | Fires the current weapon |
| `StopFire` | `void` | None | Stops firing the current weapon |
| `Reload` | `void` | None | Reloads the current weapon |
| `SwitchWeapon` | `void` | `int32 WeaponIndex` | Switches to the specified weapon |
| `Sprint` | `void` | None | Starts sprinting |
| `StopSprint` | `void` | None | Stops sprinting |
| `Aim` | `void` | None | Starts aiming |
| `StopAim` | `void` | None | Stops aiming |

### Movement Component

`UPulseFireMovementComponent` - Custom movement component for enhanced player movement.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `WalkSpeed` | `float` | Normal walking speed |
| `SprintSpeed` | `float` | Sprinting speed |
| `AimWalkSpeed` | `float` | Walking speed while aiming |
| `bIsAiming` | `bool` | Whether the character is aiming |
| `bWantsToSprint` | `bool` | Whether the character wants to sprint |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `StartSprint` | `void` | None | Starts sprinting |
| `StopSprint` | `void` | None | Stops sprinting |
| `SetAiming` | `void` | `bool bNewAiming` | Sets aiming state |
| `CanSprint` | `bool` | None | Checks if sprinting is possible |

## Weapon System

### Base Weapon

`ABaseWeapon` - Base class for all weapons in the game.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `WeaponType` | `EWeaponType` | Type of weapon |
| `FireMode` | `EFireMode` | Firing mode (single, burst, auto) |
| `BaseDamage` | `float` | Base damage per shot |
| `RateOfFire` | `float` | Shots per minute |
| `MaxAmmo` | `int32` | Maximum ammo per magazine |
| `MaxReserveAmmo` | `int32` | Maximum reserve ammo |
| `CurrentAmmo` | `int32` | Current ammo in magazine |
| `CurrentReserveAmmo` | `int32` | Current reserve ammo |
| `HipFireSpread` | `float` | Accuracy spread when firing from hip |
| `AimSpread` | `float` | Accuracy spread when aiming |
| `ReloadTime` | `float` | Time to reload in seconds |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `Fire` | `void` | None | Fires the weapon |
| `StopFire` | `void` | None | Stops firing the weapon |
| `Reload` | `void` | None | Reloads the weapon |
| `CanFire` | `bool` | None | Checks if the weapon can fire |
| `CanReload` | `bool` | None | Checks if the weapon can reload |
| `SetAiming` | `void` | `bool bNewAiming` | Sets aiming state |
| `AddAmmo` | `void` | `int32 AmmoAmount` | Adds ammo to reserve |

### Weapon Factory

`UWeaponFactory` - Factory class for creating weapon instances.

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `CreateWeapon` | `ABaseWeapon*` | `UWorld* World, EWeaponType WeaponType, FVector Location, FRotator Rotation` | Creates a weapon of the specified type |
| `GetWeaponClass` | `TSubclassOf<ABaseWeapon>` | `EWeaponType WeaponType` | Gets the class for a weapon type |

## Multiplayer System

### PulseFire Game Mode

`APulseFireGameMode` - Base game mode class for multiplayer matches.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `MatchState` | `EMatchState` | Current state of the match |
| `MatchTime` | `float` | Time limit for the match |
| `RespawnDelay` | `float` | Delay before respawning |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `StartMatch` | `void` | None | Starts the match |
| `EndMatch` | `void` | None | Ends the match |
| `RespawnPlayer` | `void` | `AController* Controller` | Respawns a player |
| `GetMatchTimeRemaining` | `float` | None | Gets remaining match time |

### Player State

`APulseFirePlayerState` - Stores player information for multiplayer.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `Kills` | `int32` | Number of kills |
| `Deaths` | `int32` | Number of deaths |
| `Score` | `float` | Player's score |
| `TeamIndex` | `int32` | Player's team index |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `AddKill` | `void` | None | Increments kill count |
| `AddDeath` | `void` | None | Increments death count |
| `AddScore` | `void` | `float ScoreAmount` | Adds to player's score |
| `SetTeamIndex` | `void` | `int32 NewTeamIndex` | Sets player's team |

## UI System

### PulseFire User Widget

`UPulseFireUserWidget` - Base class for all UI widgets.

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `Initialize` | `void` | None | Initializes the widget |
| `Show` | `void` | None | Shows the widget |
| `Hide` | `void` | None | Hides the widget |
| `PlayAnimation` | `void` | `UWidgetAnimation* Animation, float StartAtTime, int32 NumLoops, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed` | Plays a widget animation |

### HUD Widget

`UHUDWidget` - Displays in-game information.

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `UpdateHealth` | `void` | `float HealthPercent` | Updates health display |
| `UpdateAmmo` | `void` | `int32 CurrentAmmo, int32 ReserveAmmo` | Updates ammo display |
| `UpdateScore` | `void` | `int32 Score` | Updates score display |
| `UpdateTimer` | `void` | `float RemainingTime` | Updates timer display |
| `ShowHitMarker` | `void` | None | Shows hit marker animation |

## Audio System

### Sound Manager

`USoundManager` - Manages game audio.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `MasterVolume` | `float` | Master volume level |
| `MusicVolume` | `float` | Music volume level |
| `SFXVolume` | `float` | Sound effects volume level |
| `UIVolume` | `float` | UI sounds volume level |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `PlaySoundAtLocation` | `UAudioComponent*` | `const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings, USceneComponent* AttachToComponent, FName AttachPointName, FVector LocationOffset, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed` | Plays a sound at a location |
| `PlaySoundAttached` | `UAudioComponent*` | `USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings` | Plays a sound attached to a component |
| `PlaySound2D` | `UAudioComponent*` | `const UObject* WorldContextObject, USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundConcurrency* ConcurrencySettings` | Plays a 2D sound |
| `PlayMusic` | `UAudioComponent*` | `const UObject* WorldContextObject, USoundBase* Music, float FadeInDuration, float VolumeMultiplier` | Plays background music |
| `StopMusic` | `void` | `UAudioComponent* MusicComponent, float FadeOutDuration` | Stops background music |
| `SetMasterVolume` | `void` | `float Volume` | Sets master volume |
| `SetMusicVolume` | `void` | `float Volume` | Sets music volume |
| `SetSFXVolume` | `void` | `float Volume` | Sets SFX volume |
| `SetUIVolume` | `void` | `float Volume` | Sets UI volume |

### Music Manager

`AMusicManager` - Manages background music.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `CurrentMusicState` | `EMusicState` | Current music state |
| `MainMenuMusic` | `USoundBase*` | Main menu music |
| `GameplayMusic` | `USoundBase*` | Gameplay music |
| `CombatMusic` | `USoundBase*` | Combat music |
| `VictoryMusic` | `USoundBase*` | Victory music |
| `DefeatMusic` | `USoundBase*` | Defeat music |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `SetMusicState` | `void` | `EMusicState NewState` | Sets the music state |
| `NotifyCombat` | `void` | None | Notifies of combat to trigger combat music |
| `GetInstance` | `AMusicManager*` | `UObject* WorldContextObject` | Gets the singleton instance |

## Visual Effects System

### VFX Manager

`UVFXManager` - Manages visual effects.

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `SpawnEmitterAtLocation` | `UParticleSystemComponent*` | `const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, const FVector& Location, const FRotator& Rotation, const FVector& Scale, bool bAutoDestroy, EPSCPoolMethod PoolingMethod` | Spawns a particle effect at a location |
| `SpawnEmitterAttached` | `UParticleSystemComponent*` | `UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, EPSCPoolMethod PoolingMethod` | Spawns a particle effect attached to a component |
| `SpawnMuzzleFlash` | `UParticleSystemComponent*` | `UParticleSystem* MuzzleFlashTemplate, USceneComponent* AttachToComponent, FName AttachPointName` | Spawns a muzzle flash effect |
| `SpawnImpactEffect` | `UParticleSystemComponent*` | `const UObject* WorldContextObject, UParticleSystem* ImpactTemplate, const FVector& Location, const FVector& Normal` | Spawns an impact effect |
| `SpawnBulletTrail` | `UParticleSystemComponent*` | `const UObject* WorldContextObject, UParticleSystem* TrailTemplate, const FVector& StartLocation, const FVector& EndLocation` | Spawns a bullet trail effect |
| `SpawnBloodEffect` | `UParticleSystemComponent*` | `const UObject* WorldContextObject, UParticleSystem* BloodTemplate, const FVector& Location, const FVector& Normal` | Spawns a blood effect |
| `SpawnExplosionEffect` | `UParticleSystemComponent*` | `const UObject* WorldContextObject, UParticleSystem* ExplosionTemplate, const FVector& Location` | Spawns an explosion effect |

### Post-Process Manager

`UPostProcessManager` - Manages post-processing effects.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `HitFeedbackMaterial` | `UMaterialInterface*` | Material for hit feedback effect |
| `LowHealthMaterial` | `UMaterialInterface*` | Material for low health effect |
| `DeathMaterial` | `UMaterialInterface*` | Material for death effect |
| `HitFeedbackIntensity` | `float` | Intensity of hit feedback effect |
| `HitFeedbackDuration` | `float` | Duration of hit feedback effect |
| `LowHealthThreshold` | `float` | Threshold for low health effect |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `ShowHitFeedback` | `void` | `float Intensity` | Shows hit feedback effect |
| `ShowLowHealthEffect` | `void` | `float HealthPercent` | Shows low health effect |
| `ShowDeathEffect` | `void` | None | Shows death effect |

## Performance System

### Performance Profiler

`UPerformanceProfiler` - Profiles and optimizes game performance.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `UpdateInterval` | `float` | Interval for updating metrics |
| `bDetailedProfiling` | `bool` | Whether to enable detailed profiling |
| `bShowDebugOverlay` | `bool` | Whether to show debug overlay |
| `CurrentMetrics` | `FPerformanceMetrics` | Current performance metrics |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `GetPerformanceMetrics` | `FPerformanceMetrics` | None | Gets current performance metrics |
| `SetUpdateInterval` | `void` | `float NewInterval` | Sets update interval |
| `SetDetailedProfiling` | `void` | `bool bEnabled` | Enables/disables detailed profiling |
| `SetDebugOverlay` | `void` | `bool bEnabled` | Shows/hides debug overlay |
| `EnableAutoOptimization` | `void` | `bool bEnabled` | Enables/disables auto-optimization |

### LOD Manager

`ALODManager` - Manages level of detail for optimization.

#### Properties

| Name | Type | Description |
|------|------|-------------|
| `LODSettings` | `FLODSettings` | LOD settings |
| `UpdateInterval` | `float` | Interval for updating LOD |
| `bAutoAdjustLOD` | `bool` | Whether to auto-adjust LOD based on performance |
| `TargetFrameRate` | `float` | Target frame rate for auto-adjustment |

#### Methods

| Name | Return Type | Parameters | Description |
|------|-------------|------------|-------------|
| `GetLODSettings` | `FLODSettings` | None | Gets current LOD settings |
| `SetLODSettings` | `void` | `const FLODSettings& NewSettings` | Sets LOD settings |
| `SetUpdateInterval` | `void` | `float NewInterval` | Sets update interval |
| `EnableAutoAdjustLOD` | `void` | `bool bEnabled` | Enables/disables auto-adjustment |
| `SetTargetFrameRate` | `void` | `float NewFrameRate` | Sets target frame rate |
| `GetInstance` | `ALODManager*` | `UObject* WorldContextObject` | Gets the singleton instance |
