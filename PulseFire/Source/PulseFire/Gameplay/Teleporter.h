#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USoundBase;

/**
 * Teleporter that moves players to a target location.
 */
UCLASS()
class PULSEFIRE_API ATeleporter : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ATeleporter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    /** Mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    /** Particle system component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    /** Target teleporter */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Teleporter")
    ATeleporter* TargetTeleporter;

    /** Teleport sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleporter")
    USoundBase* TeleportSound;

    /** Cooldown time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleporter")
    float CooldownTime;

    /** Whether the teleporter is on cooldown */
    UPROPERTY(BlueprintReadOnly, Category = "Teleporter")
    bool bIsOnCooldown;

    /** Timer handle for cooldown */
    FTimerHandle TimerHandle_Cooldown;

    /** Called when something overlaps the collision component */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Teleport the actor to the target teleporter */
    UFUNCTION(BlueprintCallable, Category = "Teleporter")
    void TeleportActor(AActor* ActorToTeleport);

    /** Play teleport effects */
    UFUNCTION(BlueprintCallable, Category = "Teleporter")
    void PlayTeleportEffects();

    /** Reset cooldown */
    UFUNCTION()
    void ResetCooldown();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
