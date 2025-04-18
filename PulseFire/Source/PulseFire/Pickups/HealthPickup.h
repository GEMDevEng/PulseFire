#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "HealthPickup.generated.h"

/**
 * Health pickup that restores player health.
 */
UCLASS()
class PULSEFIRE_API AHealthPickup : public ABasePickup
{
    GENERATED_BODY()
    
public:
    AHealthPickup();

protected:
    /** Amount of health to restore */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    float HealthAmount;

    /** Called when the pickup is collected */
    virtual void OnPickedUp_Implementation(APulseFireCharacter* Character) override;
};
