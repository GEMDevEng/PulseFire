#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "AssaultRifle.generated.h"

/**
 * Assault Rifle weapon class
 */
UCLASS()
class PULSEFIRE_API AAssaultRifle : public ABaseWeapon
{
    GENERATED_BODY()
    
public:
    // Sets default values for this actor's properties
    AAssaultRifle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Recoil pattern for assault rifle */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TArray<FVector2D> RecoilPattern;

    /** Current position in recoil pattern */
    int32 CurrentRecoilIndex;

    /** Apply assault rifle specific recoil */
    virtual void ApplyRecoil() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
