#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VFXManager.generated.h"

class UParticleSystem;
class UNiagaraSystem;
class UMaterialInterface;
class UDecalComponent;

/**
 * Manager for handling visual effects.
 */
UCLASS(Blueprintable, BlueprintType)
class PULSEFIRE_API UVFXManager : public UObject
{
    GENERATED_BODY()
    
public:
    UVFXManager();

    /** Spawn a particle effect at a location */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnEmitterAtLocation(const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator, const FVector& Scale = FVector(1.0f), bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::AutoRelease);

    /** Spawn a particle effect attached to a component */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.0f), EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::AutoRelease);

    /** Spawn a Niagara effect at a location */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UNiagaraComponent* SpawnNiagaraSystemAtLocation(const UObject* WorldContextObject, UNiagaraSystem* SystemTemplate, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator, const FVector& Scale = FVector(1.0f), bool bAutoDestroy = true);

    /** Spawn a Niagara effect attached to a component */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UNiagaraComponent* SpawnNiagaraSystemAttached(UNiagaraSystem* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.0f), EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true);

    /** Spawn a decal at a location */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UDecalComponent* SpawnDecalAtLocation(const UObject* WorldContextObject, UMaterialInterface* DecalMaterial, const FVector& DecalSize, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator, float LifeSpan = 0.0f);

    /** Spawn a decal attached to a component */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UDecalComponent* SpawnDecalAttached(UMaterialInterface* DecalMaterial, const FVector& DecalSize, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, float LifeSpan = 0.0f);

    /** Spawn a muzzle flash effect */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnMuzzleFlash(UParticleSystem* MuzzleFlashTemplate, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None);

    /** Spawn a bullet impact effect */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnImpactEffect(const UObject* WorldContextObject, UParticleSystem* ImpactTemplate, const FVector& Location, const FVector& Normal);

    /** Spawn a bullet trail effect */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnBulletTrail(const UObject* WorldContextObject, UParticleSystem* TrailTemplate, const FVector& StartLocation, const FVector& EndLocation);

    /** Spawn a blood effect */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnBloodEffect(const UObject* WorldContextObject, UParticleSystem* BloodTemplate, const FVector& Location, const FVector& Normal);

    /** Spawn an explosion effect */
    UFUNCTION(BlueprintCallable, Category = "VFX")
    static UParticleSystemComponent* SpawnExplosionEffect(const UObject* WorldContextObject, UParticleSystem* ExplosionTemplate, const FVector& Location);
};
