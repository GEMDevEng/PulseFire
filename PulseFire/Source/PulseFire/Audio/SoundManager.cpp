#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Components/AudioComponent.h"

// Initialize static variables
UAudioComponent* USoundManager::CurrentMusicComponent = nullptr;
float USoundManager::MasterVolume = 1.0f;
float USoundManager::MusicVolume = 1.0f;
float USoundManager::SFXVolume = 1.0f;
float USoundManager::UIVolume = 1.0f;

USoundManager::USoundManager()
{
    // Load sound classes and mixes
    static ConstructorHelpers::FObjectFinder<USoundMix> MasterSoundMixObj(TEXT("/Game/Audio/SM_Master"));
    if (MasterSoundMixObj.Succeeded())
    {
        MasterSoundMix = MasterSoundMixObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundClassObj(TEXT("/Game/Audio/SC_Master"));
    if (MasterSoundClassObj.Succeeded())
    {
        MasterSoundClass = MasterSoundClassObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> MusicSoundClassObj(TEXT("/Game/Audio/SC_Music"));
    if (MusicSoundClassObj.Succeeded())
    {
        MusicSoundClass = MusicSoundClassObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> SFXSoundClassObj(TEXT("/Game/Audio/SC_SFX"));
    if (SFXSoundClassObj.Succeeded())
    {
        SFXSoundClass = SFXSoundClassObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> UISoundClassObj(TEXT("/Game/Audio/SC_UI"));
    if (UISoundClassObj.Succeeded())
    {
        UISoundClass = UISoundClassObj.Object;
    }
}

void USoundManager::Initialize()
{
    // Apply sound mix
    if (MasterSoundMix)
    {
        UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
    }

    // Apply volume settings
    SetMasterVolume(MasterVolume);
    SetMusicVolume(MusicVolume);
    SetSFXVolume(SFXVolume);
    SetUIVolume(UIVolume);
}

UAudioComponent* USoundManager::PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings, USceneComponent* AttachToComponent, FName AttachPointName, FVector LocationOffset, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed)
{
    if (!Sound)
    {
        return nullptr;
    }

    // Apply SFX volume
    VolumeMultiplier *= SFXVolume;

    // Play sound
    return UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, Sound, Location, FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings);
}

UAudioComponent* USoundManager::PlaySoundAttached(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings)
{
    if (!Sound || !AttachToComponent)
    {
        return nullptr;
    }

    // Apply SFX volume
    VolumeMultiplier *= SFXVolume;

    // Play sound
    return UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPointName, Location, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings);
}

UAudioComponent* USoundManager::PlaySound2D(const UObject* WorldContextObject, USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundConcurrency* ConcurrencySettings)
{
    if (!Sound)
    {
        return nullptr;
    }

    // Apply SFX volume
    VolumeMultiplier *= SFXVolume;

    // Play sound
    return UGameplayStatics::SpawnSound2D(WorldContextObject, Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings);
}

void USoundManager::PlayUISound(USoundBase* Sound)
{
    if (!Sound)
    {
        return;
    }

    // Play UI sound
    UGameplayStatics::PlaySound2D(Sound, UIVolume);
}

UAudioComponent* USoundManager::PlayMusic(const UObject* WorldContextObject, USoundBase* Music, float FadeInDuration, float VolumeMultiplier)
{
    if (!Music)
    {
        return nullptr;
    }

    // Stop current music
    if (CurrentMusicComponent)
    {
        StopMusic(CurrentMusicComponent, FadeInDuration);
    }

    // Apply music volume
    VolumeMultiplier *= MusicVolume;

    // Play music
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(WorldContextObject, Music, 0.0f);
    if (AudioComponent)
    {
        // Fade in
        AudioComponent->FadeIn(FadeInDuration, VolumeMultiplier);
        
        // Set as current music
        CurrentMusicComponent = AudioComponent;
    }

    return AudioComponent;
}

void USoundManager::StopMusic(UAudioComponent* MusicComponent, float FadeOutDuration)
{
    if (!MusicComponent)
    {
        return;
    }

    // Fade out and stop
    MusicComponent->FadeOut(FadeOutDuration, 0.0f);

    // Clear current music if this is the current music
    if (MusicComponent == CurrentMusicComponent)
    {
        CurrentMusicComponent = nullptr;
    }
}

void USoundManager::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    // Apply to sound class
    if (GEngine && GEngine->GetMainAudioDevice() && MasterSoundClass)
    {
        GEngine->GetMainAudioDevice()->SetSoundClassVolume(MasterSoundClass, MasterVolume);
    }
}

void USoundManager::SetMusicVolume(float Volume)
{
    MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    // Apply to sound class
    if (GEngine && GEngine->GetMainAudioDevice() && MusicSoundClass)
    {
        GEngine->GetMainAudioDevice()->SetSoundClassVolume(MusicSoundClass, MusicVolume);
    }

    // Apply to current music
    if (CurrentMusicComponent)
    {
        CurrentMusicComponent->SetVolumeMultiplier(MusicVolume);
    }
}

void USoundManager::SetSFXVolume(float Volume)
{
    SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    // Apply to sound class
    if (GEngine && GEngine->GetMainAudioDevice() && SFXSoundClass)
    {
        GEngine->GetMainAudioDevice()->SetSoundClassVolume(SFXSoundClass, SFXVolume);
    }
}

void USoundManager::SetUIVolume(float Volume)
{
    UIVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    // Apply to sound class
    if (GEngine && GEngine->GetMainAudioDevice() && UISoundClass)
    {
        GEngine->GetMainAudioDevice()->SetSoundClassVolume(UISoundClass, UIVolume);
    }
}

float USoundManager::GetMasterVolume()
{
    return MasterVolume;
}

float USoundManager::GetMusicVolume()
{
    return MusicVolume;
}

float USoundManager::GetSFXVolume()
{
    return SFXVolume;
}

float USoundManager::GetUIVolume()
{
    return UIVolume;
}
