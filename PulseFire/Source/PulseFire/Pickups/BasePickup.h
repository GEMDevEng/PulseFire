#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;
class UParticleSystemComponent;
class USoundBase;
class APulseFireCharacter;

/**
 * Base class for all pickups in the game.
 */
UCLASS(Abstract)
class PULSEFIRE_API ABasePickup : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ABasePickup();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    /** Mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    /** Rotating movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    URotatingMovementComponent* RotatingMovement;

    /** Particle system component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    /** Pickup sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    USoundBase* PickupSound;

    /** Respawn time in seconds (0 = no respawn) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
    float RespawnTime;

    /** Whether the pickup is active */
    UPROPERTY(ReplicatedUsing = OnRep_IsActive, BlueprintReadOnly, Category = "Pickup")
    bool bIsActive;

    /** Timer handle for respawn */
    FTimerHandle TimerHandle_Respawn;

    /** Called when bIsActive is replicated */
    UFUNCTION()
    virtual void OnRep_IsActive();

    /** Called when the pickup is collected */
    UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
    void OnPickedUp(APulseFireCharacter* Character);
    virtual void OnPickedUp_Implementation(APulseFireCharacter* Character);

    /** Called when the pickup respawns */
    UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
    void OnRespawn();
    virtual void OnRespawn_Implementation();

    /** Respawn the pickup */
    UFUNCTION()
    virtual void Respawn();

    /** Play pickup effects */
    virtual void PlayPickupEffects();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Called when something overlaps the collision component */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Get whether the pickup is active */
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    bool IsActive() const { return bIsActive; }

    /** Set whether the pickup is active */
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void SetActive(bool bNewActive);
};
