#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Shotgun.generated.h"

/**
 * Shotgun weapon class
 */
UCLASS()
class PULSEFIRE_API AShotgun : public ABaseWeapon
{
    GENERATED_BODY()
    
public:
    // Sets default values for this actor's properties
    AShotgun();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Damage falloff curve based on distance */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UCurveFloat* DamageFalloffCurve;

    /** Override fire to implement shotgun-specific behavior */
    virtual void Fire() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
