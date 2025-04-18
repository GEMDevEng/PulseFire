#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class UAnimMontage;
class UCameraShakeBase;
class APulseFireCharacter;

/**
 * Enum for different weapon types
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Rifle       UMETA(DisplayName = "Rifle"),
    Shotgun     UMETA(DisplayName = "Shotgun"),
    Pistol      UMETA(DisplayName = "Pistol"),
    SniperRifle UMETA(DisplayName = "Sniper Rifle"),
    SMG         UMETA(DisplayName = "SMG")
};

/**
 * Enum for different fire modes
 */
UENUM(BlueprintType)
enum class EFireMode : uint8
{
    Single      UMETA(DisplayName = "Single"),
    Burst       UMETA(DisplayName = "Burst"),
    Auto        UMETA(DisplayName = "Auto")
};

/**
 * Base weapon class for PulseFire.
 */
UCLASS(Abstract)
class PULSEFIRE_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABaseWeapon();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Weapon mesh (1P view) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComp;

    /** Weapon mesh (3P view) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComp3P;

    /** Weapon type */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType;

    /** Fire mode */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    EFireMode FireMode;

    /** Base damage per bullet */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float BaseDamage;

    /** Headshot damage multiplier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HeadshotMultiplier;

    /** Rate of fire in rounds per minute */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float RateOfFire;

    /** Maximum ammo capacity per magazine */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 MaxAmmo;

    /** Maximum reserve ammo */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 MaxReserveAmmo;

    /** Current ammo in magazine */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    int32 CurrentAmmo;

    /** Current reserve ammo */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    int32 CurrentReserveAmmo;

    /** Bullet spread in degrees (hip fire) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HipFireSpread;

    /** Bullet spread in degrees (aiming) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimSpread;

    /** Reload time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float ReloadTime;

    /** Equip time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float EquipTime;

    /** Range in centimeters */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Range;

    /** Number of pellets per shot (for shotguns) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 PelletsPerShot;

    /** Number of bullets per burst (for burst fire mode) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 BurstCount;

    /** Vertical recoil amount */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float VerticalRecoil;

    /** Horizontal recoil amount */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HorizontalRecoil;

    /** Recoil recovery speed */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float RecoilRecovery;

    /** Aim down sight speed multiplier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimDownSightSpeed;

    /** Field of view when aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimFOV;

    /** Whether the weapon is currently being reloaded */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    bool bIsReloading;

    /** Whether the weapon is currently being equipped */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    bool bIsEquipping;

    /** Whether the player is currently aiming down sights */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    bool bIsAiming;

    /** Whether the weapon is currently firing */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
    bool bIsFiring;

    /** Current burst count (for burst fire mode) */
    int32 CurrentBurstCount;

    /** Muzzle socket name */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName;

    /** Damage type */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;

    /** Muzzle effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* MuzzleEffect;

    /** Default impact effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* DefaultImpactEffect;

    /** Flesh impact effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* FleshImpactEffect;

    /** Tracer effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* TracerEffect;

    /** Fire sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    USoundBase* FireSound;

    /** Reload sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    USoundBase* ReloadSound;

    /** Empty magazine sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    USoundBase* EmptySound;

    /** Fire animation (1P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* FireAnimation1P;

    /** Fire animation (3P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* FireAnimation3P;

    /** Reload animation (1P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* ReloadAnimation1P;

    /** Reload animation (3P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* ReloadAnimation3P;

    /** Equip animation (1P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* EquipAnimation1P;

    /** Equip animation (3P) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UAnimMontage* EquipAnimation3P;

    /** Camera shake on fire */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UCameraShakeBase> FireCameraShake;

    /** Last time the weapon was fired */
    float LastFireTime;

    /** Derived from RateOfFire */
    float TimeBetweenShots;

    /** Server-side fire implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFire();
    bool ServerFire_Validate();
    void ServerFire_Implementation();

    /** Server-side reload implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerReload();
    bool ServerReload_Validate();
    void ServerReload_Implementation();

    /** Server-side aiming implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetAiming(bool bNewAiming);
    bool ServerSetAiming_Validate(bool bNewAiming);
    void ServerSetAiming_Implementation(bool bNewAiming);

    /** Play weapon effects */
    virtual void PlayFireEffects(const FVector& TraceEnd);

    /** Play impact effects */
    virtual void PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint);

    /** Play reload effects */
    virtual void PlayReloadEffects();

    /** Play equip effects */
    virtual void PlayEquipEffects();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Start firing the weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartFire();

    /** Stop firing the weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StopFire();

    /** Reload the weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Reload();

    /** Set aiming state */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void SetAiming(bool bNewAiming);

    /** Equip the weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Equip(APulseFireCharacter* NewOwner);

    /** Get current ammo */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    int32 GetCurrentAmmo() const;

    /** Get maximum ammo */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    int32 GetMaxAmmo() const;

    /** Get current reserve ammo */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    int32 GetCurrentReserveAmmo() const;

    /** Get maximum reserve ammo */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    int32 GetMaxReserveAmmo() const;

    /** Add ammo to reserve */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AddAmmo(int32 AmmoAmount);

    /** Get weapon name */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FString GetWeaponName() const;

    /** Get weapon type */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    EWeaponType GetWeaponType() const;

    /** Get fire mode */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    EFireMode GetFireMode() const;

    /** Get whether the weapon is reloading */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool IsReloading() const;

    /** Get whether the weapon is being equipped */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool IsEquipping() const;

    /** Get whether the weapon is aiming */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool IsAiming() const;

    /** Get whether the weapon is firing */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool IsFiring() const;

private:
    /** Fire the weapon */
    virtual void Fire();

    /** Handle reload completion */
    UFUNCTION()
    virtual void FinishReload();

    /** Handle equip completion */
    UFUNCTION()
    virtual void FinishEquip();

    /** Apply recoil to the player's camera */
    virtual void ApplyRecoil();

    /** Get current bullet spread based on state */
    virtual float GetCurrentSpread() const;

    /** Get the owning character */
    APulseFireCharacter* GetPulseFireCharacter() const;

    /** Timer handle for automatic fire */
    FTimerHandle TimerHandle_AutoFire;

    /** Timer handle for reload */
    FTimerHandle TimerHandle_Reload;

    /** Timer handle for equip */
    FTimerHandle TimerHandle_Equip;

    /** Current recoil amount */
    float CurrentRecoil;

    /** Weapon display name */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FString WeaponName;
};
