// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PulseFireCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

/**
 * The player character for PulseFire.
 * Handles movement, shooting, and health.
 */
UCLASS(config=Game)
class APulseFireCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Weapon mesh (first person view) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

public:
	APulseFireCharacter();

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called for movement input */
	void MoveForward(float Value);
	void MoveRight(float Value);

	/** Called for looking input */
	void LookUp(float Value);
	void Turn(float Value);

	/** Called for crouch input */
	void StartCrouch();
	void StopCrouch();

	/** Called for jump input */
	virtual void Jump() override;
	virtual void StopJumping() override;

	/** Called for fire input */
	void StartFire();
	void StopFire();

	/** Called for reload input */
	void Reload();

	/** Handles firing logic */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	bool ServerFire_Validate();
	void ServerFire_Implementation();

	/** Performs a line trace to determine hit */
	FHitResult PerformLineTrace() const;

public:	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the camera location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	uint8 bUsingMotionControllers : 1;

	/** Current health of the character */
	UPROPERTY(Replicated, BlueprintReadOnly, Category=Gameplay)
	float Health;

	/** Maximum health of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	float MaxHealth;

	/** Current ammo in magazine */
	UPROPERTY(Replicated, BlueprintReadOnly, Category=Gameplay)
	int32 CurrentAmmo;

	/** Maximum ammo per magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	int32 MaxAmmo;

	/** Whether the player is currently firing */
	UPROPERTY(Replicated, BlueprintReadOnly, Category=Gameplay)
	bool bIsFiring;

	/** Apply damage to this character */
	UFUNCTION(BlueprintCallable, Category=Gameplay)
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Called when the character dies */
	UFUNCTION(BlueprintNativeEvent, Category=Gameplay)
	void OnDeath();
	virtual void OnDeath_Implementation();

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
