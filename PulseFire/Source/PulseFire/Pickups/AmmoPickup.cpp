#include "AmmoPickup.h"
#include "../PulseFireCharacter.h"
#include "../Weapons/BaseWeapon.h"

AAmmoPickup::AAmmoPickup()
{
    // Set default values
    WeaponType = EWeaponType::Rifle;
    AmmoAmount = 30;

    // Set mesh and material
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObj.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshObj.Object);
        MeshComp->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.5f));
    }

    // Set material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/Materials/M_AmmoPickup"));
    if (MaterialObj.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialObj.Object);
    }
}

void AAmmoPickup::OnPickedUp_Implementation(APulseFireCharacter* Character)
{
    if (Character)
    {
        // Get current weapon
        ABaseWeapon* Weapon = Character->CurrentWeapon;
        if (Weapon)
        {
            // Check if weapon type matches
            if (Weapon->GetWeaponType() == WeaponType || WeaponType == EWeaponType::Rifle) // Rifle ammo is universal
            {
                // Only pickup if ammo is not full
                if (Weapon->GetCurrentReserveAmmo() < Weapon->GetMaxReserveAmmo())
                {
                    // Add ammo to the weapon
                    Weapon->AddAmmo(AmmoAmount);

                    // Call parent implementation
                    Super::OnPickedUp_Implementation(Character);
                }
            }
        }
    }
}
