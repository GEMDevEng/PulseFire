#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "../Weapons/BaseWeapon.h"
#include "WeaponPickup.generated.h"

/**
 * Weapon pickup that gives the player a new weapon.
 */
UCLASS()
class PULSEFIRE_API AWeaponPickup : public ABasePickup
{
    GENERATED_BODY()
    
public:
    AWeaponPickup();

protected:
    /** Weapon type to give */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    EWeaponType WeaponType;

    /** Weapon mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    /** Called when the pickup is collected */
    virtual void OnPickedUp_Implementation(APulseFireCharacter* Character) override;

    /** Called when the game starts */
    virtual void BeginPlay() override;
};
