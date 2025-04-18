#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class UAnimMontage;

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

	/** Weapon mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	/** Damage amount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	/** Rate of fire in rounds per minute */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	/** Maximum ammo capacity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo;

	/** Current ammo in magazine */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	/** Bullet spread in degrees */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

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

	/** Fire animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* FireAnimation;

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

	/** Get current ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentAmmo() const;

	/** Get maximum ammo */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetMaxAmmo() const;

private:
	/** Fire the weapon */
	void Fire();

	/** Timer handle for automatic fire */
	FTimerHandle TimerHandle_AutoFire;
};
