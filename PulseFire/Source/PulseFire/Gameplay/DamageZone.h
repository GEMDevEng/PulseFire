#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UDamageType;

/**
 * Damage zone that applies damage to players inside it.
 */
UCLASS()
class PULSEFIRE_API ADamageZone : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ADamageZone();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionComp;

    /** Mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    /** Particle system component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    /** Damage amount per second */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float DamagePerSecond;

    /** Damage type */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UDamageType> DamageType;

    /** Actors currently in the damage zone */
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    TArray<AActor*> ActorsInZone;

    /** Timer handle for damage */
    FTimerHandle TimerHandle_Damage;

    /** Called when something overlaps the collision component */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Called when something stops overlapping the collision component */
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /** Apply damage to actors in the zone */
    UFUNCTION()
    void ApplyDamage();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
