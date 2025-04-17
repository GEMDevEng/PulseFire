#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;
class UParticleSystemComponent;
class USoundBase;

/**
 * Jump pad that launches players in a specific direction.
 */
UCLASS()
class PULSEFIRE_API AJumpPad : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AJumpPad();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionComp;

    /** Mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    /** Arrow component to show launch direction */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* ArrowComp;

    /** Particle system component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    /** Launch sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JumpPad")
    USoundBase* LaunchSound;

    /** Launch strength */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JumpPad")
    float LaunchStrength;

    /** Launch angle (degrees) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JumpPad")
    float LaunchAngle;

    /** Called when something overlaps the collision component */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Launch the actor */
    UFUNCTION(BlueprintCallable, Category = "JumpPad")
    void LaunchActor(AActor* ActorToLaunch);

    /** Play launch effects */
    UFUNCTION(BlueprintCallable, Category = "JumpPad")
    void PlayLaunchEffects();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
