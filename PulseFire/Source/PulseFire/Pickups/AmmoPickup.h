#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "../Weapons/BaseWeapon.h"
#include "AmmoPickup.generated.h"

/**
 * Ammo pickup that restores weapon ammo.
 */
UCLASS()
class PULSEFIRE_API AAmmoPickup : public ABasePickup
{
    GENERATED_BODY()
    
public:
    AAmmoPickup();

protected:
    /** Weapon type this ammo is for */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    EWeaponType WeaponType;

    /** Amount of ammo to give */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    int32 AmmoAmount;

    /** Called when the pickup is collected */
    virtual void OnPickedUp_Implementation(APulseFireCharacter* Character) override;
};
