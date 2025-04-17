#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "../PulseFireCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

// Console variable for debug
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
    TEXT("PULSEFIRE.DebugWeapons"),
    DebugWeaponDrawing,
    TEXT("Draw Debug Lines for Weapons"),
    ECVF_Cheat);

// Sets default values
ABaseWeapon::ABaseWeapon()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create mesh components
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    
    MeshComp3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp3P"));
    MeshComp3P->SetupAttachment(RootComponent);
    MeshComp3P->SetOwnerNoSee(true);
    MeshComp3P->SetCastShadow(true);

    // Set default values
    MuzzleSocketName = "MuzzleSocket";
    WeaponType = EWeaponType::Rifle;
    FireMode = EFireMode::Single;
    BaseDamage = 20.0f;
    HeadshotMultiplier = 2.0f;
    RateOfFire = 600.0f;
    MaxAmmo = 30;
    MaxReserveAmmo = 90;
    CurrentAmmo = MaxAmmo;
    CurrentReserveAmmo = MaxReserveAmmo;
    HipFireSpread = 3.0f;
    AimSpread = 0.5f;
    ReloadTime = 2.0f;
    EquipTime = 1.0f;
    Range = 10000.0f;
    PelletsPerShot = 1;
    BurstCount = 3;
    VerticalRecoil = 1.0f;
    HorizontalRecoil = 0.5f;
    RecoilRecovery = 5.0f;
    AimDownSightSpeed = 0.5f;
    AimFOV = 60.0f;
    bIsReloading = false;
    bIsEquipping = false;
    bIsAiming = false;
    bIsFiring = false;
    CurrentBurstCount = 0;
    CurrentRecoil = 0.0f;
    WeaponName = "Base Weapon";

    // Calculate time between shots
    TimeBetweenShots = 60.0f / RateOfFire;

    // Set replication
    SetReplicates(true);
    NetUpdateFrequency = 66.0f;
    MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    // Calculate time between shots
    TimeBetweenShots = 60.0f / RateOfFire;
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Handle recoil recovery
    if (CurrentRecoil > 0.0f)
    {
        CurrentRecoil = FMath::Max(0.0f, CurrentRecoil - (RecoilRecovery * DeltaTime));
    }
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(ABaseWeapon, CurrentAmmo);
    DOREPLIFETIME(ABaseWeapon, CurrentReserveAmmo);
    DOREPLIFETIME(ABaseWeapon, bIsReloading);
    DOREPLIFETIME(ABaseWeapon, bIsEquipping);
    DOREPLIFETIME(ABaseWeapon, bIsAiming);
    DOREPLIFETIME(ABaseWeapon, bIsFiring);
}

void ABaseWeapon::StartFire()
{
    // Don't allow firing while reloading or equipping
    if (bIsReloading || bIsEquipping)
    {
        return;
    }

    // Handle different fire modes
    switch (FireMode)
    {
    case EFireMode::Single:
        // Single shot - fire once per click
        if (!bIsFiring)
        {
            bIsFiring = true;
            Fire();
        }
        break;

    case EFireMode::Burst:
        // Burst fire - fire multiple shots in quick succession
        if (!bIsFiring && CurrentBurstCount == 0)
        {
            bIsFiring = true;
            CurrentBurstCount = BurstCount;
            GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ABaseWeapon::Fire, TimeBetweenShots, true, 0.0f);
        }
        break;

    case EFireMode::Auto:
        // Automatic fire - continue firing while button is held
        bIsFiring = true;
        float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
        GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ABaseWeapon::Fire, TimeBetweenShots, true, FirstDelay);
        break;
    }
}

void ABaseWeapon::StopFire()
{
    bIsFiring = false;
    GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
}

void ABaseWeapon::Reload()
{
    // Don't allow reloading if already reloading, equipping, or at max ammo
    if (bIsReloading || bIsEquipping || CurrentAmmo == MaxAmmo || CurrentReserveAmmo <= 0)
    {
        return;
    }

    // Stop firing if we're reloading
    StopFire();

    // Start reload process
    bIsReloading = true;
    PlayReloadEffects();

    // Set timer for reload completion
    GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &ABaseWeapon::FinishReload, ReloadTime, false);

    // Replicate to server if we're a client
    if (GetLocalRole() < ROLE_Authority)
    {
        ServerReload();
    }
}

void ABaseWeapon::FinishReload()
{
    // Calculate how much ammo to add
    int32 AmmoToAdd = FMath::Min(MaxAmmo - CurrentAmmo, CurrentReserveAmmo);
    
    // Update ammo counts
    CurrentAmmo += AmmoToAdd;
    CurrentReserveAmmo -= AmmoToAdd;
    
    // Reset reloading state
    bIsReloading = false;
}

void ABaseWeapon::SetAiming(bool bNewAiming)
{
    // Don't allow aiming while reloading or equipping
    if (bIsReloading || bIsEquipping)
    {
        bNewAiming = false;
    }

    // Update aiming state
    if (bIsAiming != bNewAiming)
    {
        bIsAiming = bNewAiming;

        // Update FOV on the owning character
        APulseFireCharacter* Character = GetPulseFireCharacter();
        if (Character)
        {
            UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
            if (Camera)
            {
                Camera->SetFieldOfView(bIsAiming ? AimFOV : 90.0f);
            }
        }

        // Replicate to server if we're a client
        if (GetLocalRole() < ROLE_Authority)
        {
            ServerSetAiming(bNewAiming);
        }
    }
}

void ABaseWeapon::Equip(APulseFireCharacter* NewOwner)
{
    // Set owner
    SetOwner(NewOwner);

    // Start equip process
    bIsEquipping = true;
    PlayEquipEffects();

    // Set timer for equip completion
    GetWorldTimerManager().SetTimer(TimerHandle_Equip, this, &ABaseWeapon::FinishEquip, EquipTime, false);
}

void ABaseWeapon::FinishEquip()
{
    // Reset equipping state
    bIsEquipping = false;
}

void ABaseWeapon::Fire()
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

        // For shotguns, fire multiple pellets
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

                // Apply damage
                float ActualDamage = BaseDamage;
                
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

                // Play impact effects
                PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

                // Update tracer endpoint
                TracerEndPoint = Hit.ImpactPoint;
            }

            // Debug
            if (DebugWeaponDrawing > 0)
            {
                DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
            }

            // Play fire effects
            PlayFireEffects(TracerEndPoint);
        }

        // Apply recoil
        ApplyRecoil();

        // Update last fire time
        LastFireTime = GetWorld()->TimeSeconds;

        // For burst fire, decrement burst count and stop if done
        if (FireMode == EFireMode::Burst)
        {
            CurrentBurstCount--;
            if (CurrentBurstCount <= 0)
            {
                CurrentBurstCount = 0;
                StopFire();
            }
        }
    }
}

void ABaseWeapon::PlayFireEffects(const FVector& TraceEnd)
{
    // Play muzzle effect
    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
    }

    // Play fire sound
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // Play tracer effect
    if (TracerEffect)
    {
        FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
        UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
        if (TracerComp)
        {
            TracerComp->SetVectorParameter("Target", TraceEnd);
        }
    }

    // Play fire animation
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        // Play 1P animation
        if (FireAnimation1P)
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(FireAnimation1P, 1.0f);
            }
        }

        // Play 3P animation
        if (FireAnimation3P)
        {
            UAnimInstance* AnimInstance = MeshComp3P->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(FireAnimation3P, 1.0f);
            }
        }

        // Play camera shake
        APlayerController* PC = Cast<APlayerController>(Character->GetController());
        if (PC && FireCameraShake)
        {
            PC->ClientStartCameraShake(FireCameraShake);
        }
    }
}

void ABaseWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint)
{
    // Select impact effect based on surface type
    UParticleSystem* SelectedEffect = nullptr;
    switch (SurfaceType)
    {
    case SURFACE_FLESHDEFAULT:
    case SURFACE_FLESHVULNERABLE:
        SelectedEffect = FleshImpactEffect;
        break;
    default:
        SelectedEffect = DefaultImpactEffect;
        break;
    }

    // Spawn impact effect
    if (SelectedEffect)
    {
        FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
        FVector ShotDirection = ImpactPoint - MuzzleLocation;
        ShotDirection.Normalize();

        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
    }
}

void ABaseWeapon::PlayReloadEffects()
{
    // Play reload sound
    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
    }

    // Play reload animation
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        // Play 1P animation
        if (ReloadAnimation1P)
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(ReloadAnimation1P, 1.0f);
            }
        }

        // Play 3P animation
        if (ReloadAnimation3P)
        {
            UAnimInstance* AnimInstance = MeshComp3P->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(ReloadAnimation3P, 1.0f);
            }
        }
    }
}

void ABaseWeapon::PlayEquipEffects()
{
    // Play equip animation
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        // Play 1P animation
        if (EquipAnimation1P)
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(EquipAnimation1P, 1.0f);
            }
        }

        // Play 3P animation
        if (EquipAnimation3P)
        {
            UAnimInstance* AnimInstance = MeshComp3P->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(EquipAnimation3P, 1.0f);
            }
        }
    }
}

void ABaseWeapon::ApplyRecoil()
{
    // Add recoil
    CurrentRecoil += VerticalRecoil;

    // Apply recoil to player's camera
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        APlayerController* PC = Cast<APlayerController>(Character->GetController());
        if (PC)
        {
            // Apply vertical recoil
            PC->AddPitchInput(-VerticalRecoil * 0.05f);

            // Apply horizontal recoil (random direction)
            float HorizontalRecoilDirection = FMath::RandRange(-1.0f, 1.0f);
            PC->AddYawInput(HorizontalRecoil * HorizontalRecoilDirection * 0.05f);
        }
    }
}

float ABaseWeapon::GetCurrentSpread() const
{
    return bIsAiming ? AimSpread : HipFireSpread;
}

APulseFireCharacter* ABaseWeapon::GetPulseFireCharacter() const
{
    return Cast<APulseFireCharacter>(GetOwner());
}

bool ABaseWeapon::ServerFire_Validate()
{
    return true;
}

void ABaseWeapon::ServerFire_Implementation()
{
    Fire();
}

bool ABaseWeapon::ServerReload_Validate()
{
    return true;
}

void ABaseWeapon::ServerReload_Implementation()
{
    Reload();
}

bool ABaseWeapon::ServerSetAiming_Validate(bool bNewAiming)
{
    return true;
}

void ABaseWeapon::ServerSetAiming_Implementation(bool bNewAiming)
{
    SetAiming(bNewAiming);
}

int32 ABaseWeapon::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 ABaseWeapon::GetMaxAmmo() const
{
    return MaxAmmo;
}

int32 ABaseWeapon::GetCurrentReserveAmmo() const
{
    return CurrentReserveAmmo;
}

int32 ABaseWeapon::GetMaxReserveAmmo() const
{
    return MaxReserveAmmo;
}

FString ABaseWeapon::GetWeaponName() const
{
    return WeaponName;
}

EWeaponType ABaseWeapon::GetWeaponType() const
{
    return WeaponType;
}

EFireMode ABaseWeapon::GetFireMode() const
{
    return FireMode;
}

bool ABaseWeapon::IsReloading() const
{
    return bIsReloading;
}

bool ABaseWeapon::IsEquipping() const
{
    return bIsEquipping;
}

bool ABaseWeapon::IsAiming() const
{
    return bIsAiming;
}

bool ABaseWeapon::IsFiring() const
{
    return bIsFiring;
}
