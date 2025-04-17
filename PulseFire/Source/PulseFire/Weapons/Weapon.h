#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class UAnimMontage;
class UCameraShakeBase;
class APulseFireCharacter;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

	UPROPERTY()
	FVector_NetQuantize TraceFrom;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

	UPROPERTY()
	FVector_NetQuantize SurfaceLocation;

	UPROPERTY()
	uint8 SurfaceType;
};

/**
 * Base weapon class for PulseFire.
 */
UCLASS()
class PULSEFIRE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Weapon mesh (1P view) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	/** Weapon mesh (3P view) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp3P;

	/** Weapon data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponData WeaponData;

	/** Current ammo in magazine */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	/** Current reserve ammo */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentReserveAmmo;

	/** Whether the weapon is currently being reloaded */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading;

	/** Whether the weapon is currently being equipped */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	bool bIsEquipping;

	/** Whether the player is currently aiming down sights */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	bool bIsAiming;

	/** Current burst count (for burst fire mode) */
	int32 CurrentBurstCount;

	/** Muzzle socket name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/** Damage type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	/** Muzzle effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	/** Default impact effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	/** Flesh impact effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	/** Tracer effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	/** Fire sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundBase* FireSound;

	/** Reload sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundBase* ReloadSound;

	/** Empty magazine sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USoundBase* EmptySound;

	/** Fire animation (1P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimation1P;

	/** Fire animation (3P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimation3P;

	/** Reload animation (1P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimation1P;

	/** Reload animation (3P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* ReloadAnimation3P;

	/** Equip animation (1P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* EquipAnimation1P;

	/** Equip animation (3P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* EquipAnimation3P;

	/** Camera shake on fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	/** Last time the weapon was fired */
	float LastFireTime;

	/** Derived from RateOfFire */
	float TimeBetweenShots;

	/** Whether the weapon is currently firing */
	bool bIsFiring;

	/** Server-side fire implementation */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	bool ServerFire_Validate();
	void ServerFire_Implementation();

	/** Process the weapon trace hit */
	void ProcessHit(FHitResult& Hit, const FVector& TraceStart, const FVector& TraceEnd);

	/** Play weapon effects */
	void PlayFireEffects(const FVector& TraceEnd);

	/** Play impact effects */
	void PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint);

	/** Hit scan trace for replication */
	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	/** Replication function for hit scan trace */
	UFUNCTION()
	void OnRep_HitScanTrace();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Start firing the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	/** Stop firing the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	/** Reload the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	/** Set aiming state */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAiming(bool bNewAiming);

	/** Equip the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Equip(APulseFireCharacter* NewOwner);

	/** Get current ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentAmmo() const;

	/** Get maximum ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetMaxAmmo() const;

	/** Get current reserve ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentReserveAmmo() const;

	/** Get maximum reserve ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetMaxReserveAmmo() const;

	/** Get weapon name */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FString GetWeaponName() const;

	/** Get weapon type */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetWeaponType() const;

	/** Get fire mode */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EFireMode GetFireMode() const;

	/** Get whether the weapon is reloading */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsReloading() const;

	/** Get whether the weapon is being equipped */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsEquipping() const;

	/** Get whether the weapon is aiming */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsAiming() const;

private:
	/** Fire the weapon */
	void Fire();

	/** Handle reload completion */
	void FinishReload();

	/** Handle equip completion */
	void FinishEquip();

	/** Apply recoil to the player's camera */
	void ApplyRecoil();

	/** Get current bullet spread based on state */
	float GetCurrentSpread() const;

	/** Get the owning character */
	APulseFireCharacter* GetPulseFireCharacter() const;

	/** Timer handle for automatic fire */
	FTimerHandle TimerHandle_AutoFire;

	/** Timer handle for reload */
	FTimerHandle TimerHandle_Reload;

	/** Timer handle for equip */
	FTimerHandle TimerHandle_Equip;

	/** Current recoil amount */
	float CurrentRecoil;

	/** Weapon display name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FString WeaponName;
};
