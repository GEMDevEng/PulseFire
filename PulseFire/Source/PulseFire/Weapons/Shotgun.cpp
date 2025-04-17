#include "Shotgun.h"
#include "../PulseFireCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"

// Console variable for debug
static int32 DebugShotgunDrawing = 0;
FAutoConsoleVariableRef CVARDebugShotgunDrawing(
    TEXT("PULSEFIRE.DebugShotgun"),
    DebugShotgunDrawing,
    TEXT("Draw Debug Lines for Shotgun"),
    ECVF_Cheat);

AShotgun::AShotgun()
{
    // Set shotgun specific values
    WeaponType = EWeaponType::Shotgun;
    FireMode = EFireMode::Single;
    WeaponName = "Shotgun";
    BaseDamage = 15.0f; // Per pellet
    HeadshotMultiplier = 1.5f;
    RateOfFire = 60.0f; // 60 RPM (1 shot per second)
    MaxAmmo = 8;
    MaxReserveAmmo = 32;
    HipFireSpread = 8.0f;
    AimSpread = 5.0f;
    ReloadTime = 0.5f; // Per shell
    EquipTime = 1.2f;
    Range = 2000.0f;
    PelletsPerShot = 8;
    VerticalRecoil = 3.0f;
    HorizontalRecoil = 1.0f;
    RecoilRecovery = 3.0f;
    AimDownSightSpeed = 0.7f;
    AimFOV = 70.0f;
}

void AShotgun::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize ammo
    CurrentAmmo = MaxAmmo;
    CurrentReserveAmmo = MaxReserveAmmo;
}

void AShotgun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AShotgun::Fire()
{
    // Check if we have authority or if we're a client
    if (GetLocalRole() < ROLE_Authority)
    {
        ServerFire();
    }

    // Don't allow firing while reloading or equipping
    if (bIsReloading || bIsEquipping)
    {
        return;
    }

    // Check if we have ammo
    if (CurrentAmmo <= 0)
    {
        // Play empty sound
        if (EmptySound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetActorLocation());
        }

        // Auto-reload if we're out of ammo
        Reload();
        return;
    }

    // Consume ammo
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentAmmo--;
    }

    // Get owner and socket transform
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        // Get eye location and rotation
        FVector EyeLocation;
        FRotator EyeRotation;
        Character->GetActorEyesViewPoint(EyeLocation, EyeRotation);

        // For shotguns, fire multiple pellets in a pattern
        TArray<FVector> ImpactPoints;
        
        for (int32 i = 0; i < PelletsPerShot; i++)
        {
            // Calculate direction with random spread
            FVector ShotDirection = EyeRotation.Vector();
            
            // Apply bullet spread
            float CurrentSpreadDegrees = GetCurrentSpread();
            float HalfRad = FMath::DegreesToRadians(CurrentSpreadDegrees);
            ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

            // Calculate end point
            FVector TraceEnd = EyeLocation + (ShotDirection * Range);

            // Set trace parameters
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(Character);
            QueryParams.AddIgnoredActor(this);
            QueryParams.bTraceComplex = true;
            QueryParams.bReturnPhysicalMaterial = true;

            // Particle target parameter
            FVector TracerEndPoint = TraceEnd;

            // Hit result
            FHitResult Hit;
            bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
            
            // Process hit
            if (bHit)
            {
                // Get hit actor
                AActor* HitActor = Hit.GetActor();

                // Get surface type
                EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

                // Apply damage with falloff based on distance
                float DistanceRatio = FMath::Clamp(Hit.Distance / Range, 0.0f, 1.0f);
                float DamageFalloff = DamageFalloffCurve ? DamageFalloffCurve->GetFloatValue(DistanceRatio) : (1.0f - DistanceRatio);
                float ActualDamage = BaseDamage * DamageFalloff;
                
                // Apply headshot multiplier if applicable
                if (SurfaceType == SURFACE_FLESHVULNERABLE)
                {
                    ActualDamage *= HeadshotMultiplier;
                }

                if (GetLocalRole() == ROLE_Authority && HitActor)
                {
                    FPointDamageEvent DamageEvent(ActualDamage, Hit, ShotDirection, DamageType);
                    HitActor->TakeDamage(ActualDamage, DamageEvent, Character->GetController(), Character);
                }

                // Store impact point for effects
                ImpactPoints.Add(Hit.ImpactPoint);

                // Update tracer endpoint
                TracerEndPoint = Hit.ImpactPoint;
            }

            // Debug
            if (DebugShotgunDrawing > 0)
            {
                DrawDebugLine(GetWorld(), EyeLocation, bHit ? TracerEndPoint : TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
            }
        }

        // Play fire effects
        PlayFireEffects(EyeLocation + (EyeRotation.Vector() * Range));

        // Play impact effects for each hit
        for (const FVector& ImpactPoint : ImpactPoints)
        {
            PlayImpactEffects(SURFACE_DEFAULT, ImpactPoint);
        }

        // Apply recoil
        ApplyRecoil();

        // Update last fire time
        LastFireTime = GetWorld()->TimeSeconds;
    }
}
