#include "WeaponFactory.h"
#include "AssaultRifle.h"
#include "Shotgun.h"
#include "SniperRifle.h"
#include "../PulseFireCharacter.h"

UWeaponFactory::UWeaponFactory()
{
}

ABaseWeapon* UWeaponFactory::CreateWeapon(UWorld* World, EWeaponType WeaponType, APulseFireCharacter* Owner)
{
    if (!World)
    {
        return nullptr;
    }

    // Get the weapon class
    TSubclassOf<ABaseWeapon> WeaponClass = GetWeaponClass(WeaponType);
    if (!WeaponClass)
    {
        return nullptr;
    }

    // Spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = Owner;

    // Spawn the weapon
    ABaseWeapon* Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    if (Weapon && Owner)
    {
        // Equip the weapon
        Weapon->Equip(Owner);
    }

    return Weapon;
}

TSubclassOf<ABaseWeapon> UWeaponFactory::GetWeaponClass(EWeaponType WeaponType)
{
    switch (WeaponType)
    {
    case EWeaponType::Rifle:
        return AAssaultRifle::StaticClass();
    case EWeaponType::Shotgun:
        return AShotgun::StaticClass();
    case EWeaponType::SniperRifle:
        return ASniperRifle::StaticClass();
    // Add more weapon types as they are implemented
    default:
        return AAssaultRifle::StaticClass(); // Default to assault rifle
    }
}
