#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

/**
 * Enum for different weapon types
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Rifle       UMETA(DisplayName = "Rifle"),
    Shotgun     UMETA(DisplayName = "Shotgun"),
    Pistol      UMETA(DisplayName = "Pistol"),
    Sniper      UMETA(DisplayName = "Sniper"),
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
 * Struct containing weapon stats
 */
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()

    /** Type of weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType = EWeaponType::Rifle;

    /** Fire mode of the weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    EFireMode FireMode = EFireMode::Single;

    /** Base damage per bullet */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float BaseDamage = 20.0f;

    /** Headshot damage multiplier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HeadshotMultiplier = 2.0f;

    /** Rate of fire in rounds per minute */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float RateOfFire = 600.0f;

    /** Maximum ammo capacity per magazine */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 MaxAmmo = 30;

    /** Maximum reserve ammo */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 MaxReserveAmmo = 90;

    /** Bullet spread in degrees (hip fire) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HipFireSpread = 3.0f;

    /** Bullet spread in degrees (aiming) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimSpread = 0.5f;

    /** Reload time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float ReloadTime = 2.0f;

    /** Equip time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float EquipTime = 1.0f;

    /** Range in centimeters */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float Range = 10000.0f;

    /** Number of pellets per shot (for shotguns) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 PelletsPerShot = 1;

    /** Number of bullets per burst (for burst fire mode) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    int32 BurstCount = 3;

    /** Vertical recoil amount */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float VerticalRecoil = 1.0f;

    /** Horizontal recoil amount */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float HorizontalRecoil = 0.5f;

    /** Recoil recovery speed */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float RecoilRecovery = 5.0f;

    /** Aim down sight speed multiplier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimDownSightSpeed = 0.5f;

    /** Field of view when aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimFOV = 60.0f;
};
