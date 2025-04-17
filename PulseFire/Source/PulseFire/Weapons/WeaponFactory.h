#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseWeapon.h"
#include "WeaponFactory.generated.h"

class APulseFireCharacter;

/**
 * Factory class for creating weapons
 */
UCLASS(Blueprintable)
class PULSEFIRE_API UWeaponFactory : public UObject
{
    GENERATED_BODY()
    
public:
    UWeaponFactory();

    /** Create a weapon of the specified type */
    UFUNCTION(BlueprintCallable, Category = "Weapon Factory")
    static ABaseWeapon* CreateWeapon(UWorld* World, EWeaponType WeaponType, APulseFireCharacter* Owner);

    /** Get the weapon class for the specified type */
    UFUNCTION(BlueprintCallable, Category = "Weapon Factory")
    static TSubclassOf<ABaseWeapon> GetWeaponClass(EWeaponType WeaponType);
};
