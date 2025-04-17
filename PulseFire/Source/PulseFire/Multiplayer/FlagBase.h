#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class AFlag;
class APulseFireCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFlagCapturedAtBase, AFlag*, Flag, APulseFireCharacter*, Character, AFlagBase*, FlagBase);

/**
 * Base for flags in Capture the Flag game mode.
 */
UCLASS()
class PULSEFIRE_API AFlagBase : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AFlagBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Base mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag Base")
    UStaticMeshComponent* BaseMesh;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag Base")
    UBoxComponent* CollisionComponent;

    /** Base effect */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag Base")
    UParticleSystemComponent* BaseEffect;

    /** Team index */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flag Base")
    int32 TeamIndex;

    /** Flag */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flag Base")
    AFlag* Flag;

    /** Overlap begin event */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Get team index */
    UFUNCTION(BlueprintCallable, Category = "Flag Base")
    int32 GetTeamIndex() const { return TeamIndex; }

    /** Set team index */
    UFUNCTION(BlueprintCallable, Category = "Flag Base")
    void SetTeamIndex(int32 NewTeamIndex);

    /** Get flag */
    UFUNCTION(BlueprintCallable, Category = "Flag Base")
    AFlag* GetFlag() const { return Flag; }

    /** Set flag */
    UFUNCTION(BlueprintCallable, Category = "Flag Base")
    void SetFlag(AFlag* NewFlag);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Flag Base")
    FOnFlagCapturedAtBase OnFlagCapturedAtBase;
};
