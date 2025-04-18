#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class USphereComponent;
class APulseFireCharacter;
class AFlagBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlagPickedUp, AFlag*, Flag, APulseFireCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlagDropped, AFlag*, Flag, APulseFireCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFlagCaptured, AFlag*, Flag, APulseFireCharacter*, Character, AFlagBase*, FlagBase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlagReturned, AFlag*, Flag);

/**
 * Flag actor for Capture the Flag game mode.
 */
UCLASS()
class PULSEFIRE_API AFlag : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AFlag();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Flag mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
    UStaticMeshComponent* FlagMesh;

    /** Collision component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
    USphereComponent* CollisionComponent;

    /** Flag effect */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
    UParticleSystemComponent* FlagEffect;

    /** Team index */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flag")
    int32 TeamIndex;

    /** Flag base */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flag")
    AFlagBase* HomeBase;

    /** Flag carrier */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    APulseFireCharacter* Carrier;

    /** Whether the flag is at home base */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    bool bIsAtHome;

    /** Whether the flag is being carried */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    bool bIsBeingCarried;

    /** Whether the flag is on the ground */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Flag")
    bool bIsOnGround;

    /** Timer handle for flag return */
    FTimerHandle TimerHandle_FlagReturn;

    /** Socket name for attaching flag to carrier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flag")
    FName AttachSocketName;

    /** Overlap begin event */
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /** Return the flag to base */
    UFUNCTION()
    void ReturnToBaseInternal();

    /** Server-side pickup implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerPickup(APulseFireCharacter* Character);
    bool ServerPickup_Validate(APulseFireCharacter* Character);
    void ServerPickup_Implementation(APulseFireCharacter* Character);

    /** Server-side drop implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerDrop();
    bool ServerDrop_Validate();
    void ServerDrop_Implementation();

    /** Server-side capture implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerCapture(AFlagBase* FlagBase);
    bool ServerCapture_Validate(AFlagBase* FlagBase);
    void ServerCapture_Implementation(AFlagBase* FlagBase);

    /** Server-side return implementation */
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerReturn();
    bool ServerReturn_Validate();
    void ServerReturn_Implementation();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Get team index */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    int32 GetTeamIndex() const { return TeamIndex; }

    /** Set team index */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void SetTeamIndex(int32 NewTeamIndex);

    /** Get home base */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    AFlagBase* GetHomeBase() const { return HomeBase; }

    /** Set home base */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void SetHomeBase(AFlagBase* NewHomeBase);

    /** Get carrier */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    APulseFireCharacter* GetCarrier() const { return Carrier; }

    /** Set carrier */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void SetCarrier(APulseFireCharacter* NewCarrier);

    /** Clear carrier */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void ClearCarrier();

    /** Is the flag at home base? */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    bool IsAtHome() const { return bIsAtHome; }

    /** Is the flag being carried? */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    bool IsBeingCarried() const { return bIsBeingCarried; }

    /** Is the flag on the ground? */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    bool IsOnGround() const { return bIsOnGround; }

    /** Pickup the flag */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void Pickup(APulseFireCharacter* Character);

    /** Drop the flag */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void Drop();

    /** Capture the flag */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void Capture(AFlagBase* FlagBase);

    /** Return the flag to base */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void ReturnToBase();

    /** Start return timer */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void StartReturnTimer(float ReturnTime);

    /** Cancel return timer */
    UFUNCTION(BlueprintCallable, Category = "Flag")
    void CancelReturnTimer();

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Flag")
    FOnFlagPickedUp OnFlagPickedUp;

    UPROPERTY(BlueprintAssignable, Category = "Flag")
    FOnFlagDropped OnFlagDropped;

    UPROPERTY(BlueprintAssignable, Category = "Flag")
    FOnFlagCaptured OnFlagCaptured;

    UPROPERTY(BlueprintAssignable, Category = "Flag")
    FOnFlagReturned OnFlagReturned;
};
