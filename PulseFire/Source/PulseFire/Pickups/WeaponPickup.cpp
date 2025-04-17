#include "WeaponPickup.h"
#include "../PulseFireCharacter.h"
#include "../Weapons/BaseWeapon.h"
#include "../Weapons/WeaponFactory.h"

AWeaponPickup::AWeaponPickup()
{
    // Set default values
    WeaponType = EWeaponType::Rifle;

    // Create weapon mesh
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComponent);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Hide the default mesh
    MeshComp->SetVisibility(false);
}

void AWeaponPickup::BeginPlay()
{
    Super::BeginPlay();

    // Set the weapon mesh based on the weapon type
    TSubclassOf<ABaseWeapon> WeaponClass = UWeaponFactory::GetWeaponClass(WeaponType);
    if (WeaponClass)
    {
        ABaseWeapon* DefaultWeapon = WeaponClass->GetDefaultObject<ABaseWeapon>();
        if (DefaultWeapon)
        {
            // Get the mesh from the default weapon
            USkeletalMeshComponent* DefaultMesh = DefaultWeapon->FindComponentByClass<USkeletalMeshComponent>();
            if (DefaultMesh && DefaultMesh->SkeletalMesh)
            {
                WeaponMesh->SetSkeletalMesh(DefaultMesh->SkeletalMesh);
            }
        }
    }
}

void AWeaponPickup::OnPickedUp_Implementation(APulseFireCharacter* Character)
{
    if (Character)
    {
        // Spawn and equip the weapon
        ABaseWeapon* NewWeapon = Character->SpawnAndEquipWeapon(WeaponType);
        if (NewWeapon)
        {
            // Call parent implementation
            Super::OnPickedUp_Implementation(Character);
        }
    }
}
