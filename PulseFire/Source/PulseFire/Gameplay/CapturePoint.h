#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapturePoint.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USoundBase;
class APulseFireCharacter;
class APulseFirePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCapturePointStateChanged, int32, TeamIndex, float, CaptureProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCapturePointCaptured, int32, TeamIndex);

/**
 * Capture point for objective-based gameplay.
 */
UCLASS()
class PULSEFIRE_API ACapturePoint : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ACapturePoint();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CollisionComp;

    /** Base mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BaseMeshComp;

    /** Flag mesh component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* FlagMeshComp;

    /** Particle system component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    /** Capture sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
    USoundBase* CaptureSound;

    /** Capture progress sound */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
    USoundBase* CaptureProgressSound;

    /** Capture time in seconds */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
    float CaptureTime;

    /** Current capture progress (0.0 - 1.0) */
    UPROPERTY(ReplicatedUsing = OnRep_CaptureProgress, BlueprintReadOnly, Category = "CapturePoint")
    float CaptureProgress;

    /** Team currently capturing the point */
    UPROPERTY(ReplicatedUsing = OnRep_CapturingTeam, BlueprintReadOnly, Category = "CapturePoint")
    int32 CapturingTeam;

    /** Team that owns the point */
    UPROPERTY(ReplicatedUsing = OnRep_OwningTeam, BlueprintReadOnly, Category = "CapturePoint")
    int32 OwningTeam;

    /** Characters currently in the capture zone */
    UPROPERTY(BlueprintReadOnly, Category = "CapturePoint")
    TArray<APulseFireCharacter*> CharactersInZone;

    /** Timer handle for capture update */
    FTimerHandle TimerHandle_CaptureUpdate;

    /** Called when CaptureProgress is replicated */
    UFUNCTION()
    virtual void OnRep_CaptureProgress();

    /** Called when CapturingTeam is replicated */
    UFUNCTION()
    virtual void OnRep_CapturingTeam();

    /** Called when OwningTeam is replicated */
    UFUNCTION()
    virtual void OnRep_OwningTeam();

    /** Called when something overlaps the collision component */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Called when something stops overlapping the collision component */
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /** Update capture progress */
    UFUNCTION()
    void UpdateCapture();

    /** Play capture effects */
    UFUNCTION(BlueprintCallable, Category = "CapturePoint")
    void PlayCaptureEffects();

    /** Update visual appearance based on owning team */
    UFUNCTION(BlueprintCallable, Category = "CapturePoint")
    void UpdateAppearance();

    /** Get team counts in the capture zone */
    void GetTeamCounts(TMap<int32, int32>& TeamCounts);

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Get the owning team */
    UFUNCTION(BlueprintCallable, Category = "CapturePoint")
    int32 GetOwningTeam() const { return OwningTeam; }

    /** Get the capturing team */
    UFUNCTION(BlueprintCallable, Category = "CapturePoint")
    int32 GetCapturingTeam() const { return CapturingTeam; }

    /** Get the capture progress */
    UFUNCTION(BlueprintCallable, Category = "CapturePoint")
    float GetCaptureProgress() const { return CaptureProgress; }

    /** Delegate for capture point state changes */
    UPROPERTY(BlueprintAssignable, Category = "CapturePoint")
    FOnCapturePointStateChanged OnCapturePointStateChanged;

    /** Delegate for capture point captured */
    UPROPERTY(BlueprintAssignable, Category = "CapturePoint")
    FOnCapturePointCaptured OnCapturePointCaptured;
};
