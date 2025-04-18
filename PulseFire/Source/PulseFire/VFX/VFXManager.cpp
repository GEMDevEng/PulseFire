#include "VFXManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"

UVFXManager::UVFXManager()
{
}

UParticleSystemComponent* UVFXManager::SpawnEmitterAtLocation(const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, const FVector& Location, const FRotator& Rotation, const FVector& Scale, bool bAutoDestroy, EPSCPoolMethod PoolingMethod)
{
    if (!EmitterTemplate)
    {
        return nullptr;
    }

    return UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, EmitterTemplate, Location, Rotation, Scale, bAutoDestroy, PoolingMethod);
}

UParticleSystemComponent* UVFXManager::SpawnEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, EPSCPoolMethod PoolingMethod)
{
    if (!EmitterTemplate || !AttachToComponent)
    {
        return nullptr;
    }

    return UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, AttachToComponent, AttachPointName, Location, Rotation, Scale, LocationType, bAutoDestroy, PoolingMethod);
}

UNiagaraComponent* UVFXManager::SpawnNiagaraSystemAtLocation(const UObject* WorldContextObject, UNiagaraSystem* SystemTemplate, const FVector& Location, const FRotator& Rotation, const FVector& Scale, bool bAutoDestroy)
{
    if (!SystemTemplate)
    {
        return nullptr;
    }

    return UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, SystemTemplate, Location, Rotation, Scale, bAutoDestroy);
}

UNiagaraComponent* UVFXManager::SpawnNiagaraSystemAttached(UNiagaraSystem* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy)
{
    if (!SystemTemplate || !AttachToComponent)
    {
        return nullptr;
    }

    return UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplate, AttachToComponent, AttachPointName, Location, Rotation, LocationType, bAutoDestroy);
}

UDecalComponent* UVFXManager::SpawnDecalAtLocation(const UObject* WorldContextObject, UMaterialInterface* DecalMaterial, const FVector& DecalSize, const FVector& Location, const FRotator& Rotation, float LifeSpan)
{
    if (!DecalMaterial)
    {
        return nullptr;
    }

    return UGameplayStatics::SpawnDecalAtLocation(WorldContextObject, DecalMaterial, DecalSize, Location, Rotation, LifeSpan);
}

UDecalComponent* UVFXManager::SpawnDecalAttached(UMaterialInterface* DecalMaterial, const FVector& DecalSize, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, float LifeSpan)
{
    if (!DecalMaterial || !AttachToComponent)
    {
        return nullptr;
    }

    return UGameplayStatics::SpawnDecalAttached(DecalMaterial, DecalSize, AttachToComponent, AttachPointName, Location, Rotation, LocationType, LifeSpan);
}

UParticleSystemComponent* UVFXManager::SpawnMuzzleFlash(UParticleSystem* MuzzleFlashTemplate, USceneComponent* AttachToComponent, FName AttachPointName)
{
    if (!MuzzleFlashTemplate || !AttachToComponent)
    {
        return nullptr;
    }

    // Spawn muzzle flash
    UParticleSystemComponent* PSC = SpawnEmitterAttached(MuzzleFlashTemplate, AttachToComponent, AttachPointName);
    
    // Set auto destroy
    if (PSC)
    {
        PSC->SetAutoDestroy(true);
    }

    return PSC;
}

UParticleSystemComponent* UVFXManager::SpawnImpactEffect(const UObject* WorldContextObject, UParticleSystem* ImpactTemplate, const FVector& Location, const FVector& Normal)
{
    if (!ImpactTemplate)
    {
        return nullptr;
    }

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

UParticleSystemComponent* UVFXManager::SpawnBulletTrail(const UObject* WorldContextObject, UParticleSystem* TrailTemplate, const FVector& StartLocation, const FVector& EndLocation)
{
    if (!TrailTemplate)
    {
        return nullptr;
    }

    // Calculate rotation
    FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
    FRotator Rotation = Direction.Rotation();

    // Spawn trail effect
    UParticleSystemComponent* PSC = SpawnEmitterAtLocation(WorldContextObject, TrailTemplate, StartLocation, Rotation);
    
    // Set beam end point
    if (PSC)
    {
        PSC->SetVectorParameter(FName("EndPoint"), EndLocation);
        PSC->SetAutoDestroy(true);
    }

    return PSC;
}

UParticleSystemComponent* UVFXManager::SpawnBloodEffect(const UObject* WorldContextObject, UParticleSystem* BloodTemplate, const FVector& Location, const FVector& Normal)
{
    if (!BloodTemplate)
    {
        return nullptr;
    }

    // Calculate rotation from normal
    FRotator Rotation = Normal.Rotation();

    // Spawn blood effect
    UParticleSystemComponent* PSC = SpawnEmitterAtLocation(WorldContextObject, BloodTemplate, Location, Rotation);
    
    // Set auto destroy
    if (PSC)
    {
        PSC->SetAutoDestroy(true);
    }

    return PSC;
}

UParticleSystemComponent* UVFXManager::SpawnExplosionEffect(const UObject* WorldContextObject, UParticleSystem* ExplosionTemplate, const FVector& Location)
{
    if (!ExplosionTemplate)
    {
        return nullptr;
    }

    // Spawn explosion effect
    UParticleSystemComponent* PSC = SpawnEmitterAtLocation(WorldContextObject, ExplosionTemplate, Location);
    
    // Set auto destroy
    if (PSC)
    {
        PSC->SetAutoDestroy(true);
    }

    return PSC;
}
