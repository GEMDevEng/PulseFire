#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

class USoundBase;
class UAudioComponent;

UENUM(BlueprintType)
enum class EMusicState : uint8
{
    None        UMETA(DisplayName = "None"),
    MainMenu    UMETA(DisplayName = "Main Menu"),
    Gameplay    UMETA(DisplayName = "Gameplay"),
    Combat      UMETA(DisplayName = "Combat"),
    Victory     UMETA(DisplayName = "Victory"),
    Defeat      UMETA(DisplayName = "Defeat")
};

/**
 * Manager for handling background music.
 */
UCLASS()
class PULSEFIRE_API AMusicManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AMusicManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Main menu music */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    USoundBase* MainMenuMusic;

    /** Gameplay music */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    USoundBase* GameplayMusic;

    /** Combat music */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    USoundBase* CombatMusic;

    /** Victory music */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    USoundBase* VictoryMusic;

    /** Defeat music */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    USoundBase* DefeatMusic;

    /** Current music state */
    UPROPERTY(BlueprintReadOnly, Category = "Music")
    EMusicState CurrentMusicState;

    /** Current music component */
    UPROPERTY()
    UAudioComponent* CurrentMusicComponent;

    /** Time to fade between music tracks */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    float FadeTime;

    /** Combat timer */
    FTimerHandle TimerHandle_Combat;

    /** Combat timeout duration */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    float CombatTimeout;

    /** Reset to gameplay music after combat timeout */
    UFUNCTION()
    void ResetToGameplayMusic();

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Set the music state */
    UFUNCTION(BlueprintCallable, Category = "Music")
    void SetMusicState(EMusicState NewState);

    /** Notify of combat */
    UFUNCTION(BlueprintCallable, Category = "Music")
    void NotifyCombat();

    /** Get the singleton instance */
    UFUNCTION(BlueprintCallable, Category = "Music")
    static AMusicManager* GetInstance(UObject* WorldContextObject);
};
