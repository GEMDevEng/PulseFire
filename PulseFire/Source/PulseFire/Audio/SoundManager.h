#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

class USoundBase;
class UAudioComponent;
class USoundCue;
class USoundMix;
class USoundClass;

/**
 * Sound manager for handling game audio.
 */
UCLASS(Blueprintable, BlueprintType)
class PULSEFIRE_API USoundManager : public UObject
{
    GENERATED_BODY()
    
public:
    USoundManager();

    /** Initialize the sound manager */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void Initialize();

    /** Play a sound at a location */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static UAudioComponent* PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, USoundAttenuation* AttenuationSettings = nullptr, USoundConcurrency* ConcurrencySettings = nullptr, USceneComponent* AttachToComponent = nullptr, FName AttachPointName = NAME_None, FVector LocationOffset = FVector::ZeroVector, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bStopWhenAttachedToDestroyed = false);

    /** Play a sound attached to a component */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static UAudioComponent* PlaySoundAttached(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector::ZeroVector, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, USoundAttenuation* AttenuationSettings = nullptr, USoundConcurrency* ConcurrencySettings = nullptr);

    /** Play a 2D sound */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static UAudioComponent* PlaySound2D(const UObject* WorldContextObject, USoundBase* Sound, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f, USoundConcurrency* ConcurrencySettings = nullptr);

    /** Play a UI sound */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void PlayUISound(USoundBase* Sound);

    /** Play music */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static UAudioComponent* PlayMusic(const UObject* WorldContextObject, USoundBase* Music, float FadeInDuration = 2.0f, float VolumeMultiplier = 1.0f);

    /** Stop music */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void StopMusic(UAudioComponent* MusicComponent, float FadeOutDuration = 2.0f);

    /** Set master volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void SetMasterVolume(float Volume);

    /** Set music volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void SetMusicVolume(float Volume);

    /** Set SFX volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void SetSFXVolume(float Volume);

    /** Set UI volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static void SetUIVolume(float Volume);

    /** Get master volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static float GetMasterVolume();

    /** Get music volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static float GetMusicVolume();

    /** Get SFX volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static float GetSFXVolume();

    /** Get UI volume */
    UFUNCTION(BlueprintCallable, Category = "Audio")
    static float GetUIVolume();

private:
    /** Master sound mix */
    UPROPERTY()
    USoundMix* MasterSoundMix;

    /** Master sound class */
    UPROPERTY()
    USoundClass* MasterSoundClass;

    /** Music sound class */
    UPROPERTY()
    USoundClass* MusicSoundClass;

    /** SFX sound class */
    UPROPERTY()
    USoundClass* SFXSoundClass;

    /** UI sound class */
    UPROPERTY()
    USoundClass* UISoundClass;

    /** Current music component */
    UPROPERTY()
    static UAudioComponent* CurrentMusicComponent;

    /** Master volume */
    static float MasterVolume;

    /** Music volume */
    static float MusicVolume;

    /** SFX volume */
    static float SFXVolume;

    /** UI volume */
    static float UIVolume;
};
