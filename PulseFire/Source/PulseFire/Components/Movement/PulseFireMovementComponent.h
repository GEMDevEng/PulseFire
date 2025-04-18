#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PulseFireMovementComponent.generated.h"

/**
 * Custom movement component for PulseFire characters.
 * Extends the standard CharacterMovementComponent with FPS-specific functionality.
 */
UCLASS()
class PULSEFIRE_API UPulseFireMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPulseFireMovementComponent();

	// Override base movement functions
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Set whether the character is sprinting */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetSprinting(bool bNewSprinting);

	/** Get whether the character is sprinting */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsSprinting() const;

	/** Set whether the character is aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetAiming(bool bNewAiming);

	/** Get whether the character is aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsAiming() const;

protected:
	/** Base walking speed (cm/s) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float BaseWalkSpeed;

	/** Sprint speed multiplier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "1.0"))
	float SprintSpeedMultiplier;

	/** Aim speed multiplier (typically < 1.0 to slow down while aiming) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float AimSpeedMultiplier;

	/** Whether the character is currently sprinting */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

	/** Whether the character is currently aiming down sights */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
	bool bIsAiming;

	/** Update movement speeds based on current state */
	virtual void UpdateMovementSpeeds();

	/** Server-side function to set sprinting */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool bNewSprinting);
	bool ServerSetSprinting_Validate(bool bNewSprinting);
	void ServerSetSprinting_Implementation(bool bNewSprinting);

	/** Server-side function to set aiming */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetAiming(bool bNewAiming);
	bool ServerSetAiming_Validate(bool bNewAiming);
	void ServerSetAiming_Implementation(bool bNewAiming);
};
