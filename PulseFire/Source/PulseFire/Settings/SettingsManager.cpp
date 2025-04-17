#include "SettingsManager.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/InputSettings.h"
#include "../Audio/SoundManager.h"
#include "Engine/Engine.h"

// Initialize static instance
USettingsManager* USettingsManager::Instance = nullptr;

USettingsManager::USettingsManager()
{
    // Set default values
    GraphicsSettings = FGraphicsSettings();
    AudioSettings = FAudioSettings();
    GameplaySettings = FGameplaySettings();
    KeyBindingSettings = FKeyBindingSettings();
}

void USettingsManager::Initialize()
{
    // Load settings
    LoadSettings();

    // Apply settings
    ApplyGraphicsSettings();
    ApplyAudioSettings();
    ApplyGameplaySettings();
    ApplyKeyBindings();
}

void USettingsManager::SaveSettings()
{
    // Get game user settings
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (UserSettings)
    {
        // Save user settings
        UserSettings->SaveSettings();
    }

    // Save custom settings to config file
    UGameplayStatics::SaveGameToSlot(this, "PulseFireSettings", 0);

    // Broadcast settings changed event
    OnSettingsChanged.Broadcast();
}

void USettingsManager::LoadSettings()
{
    // Load custom settings from config file
    UGameplayStatics::LoadGameFromSlot("PulseFireSettings", 0);

    // Get current key bindings
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();
    if (InputSettings)
    {
        KeyBindingSettings.ActionMappings = InputSettings->GetActionMappings();
        KeyBindingSettings.AxisMappings = InputSettings->GetAxisMappings();
    }
}

void USettingsManager::ResetToDefaults()
{
    // Reset to default values
    GraphicsSettings = FGraphicsSettings();
    AudioSettings = FAudioSettings();
    GameplaySettings = FGameplaySettings();

    // Reset key bindings to default
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();
    if (InputSettings)
    {
        InputSettings->LoadDefaultConfig();
        KeyBindingSettings.ActionMappings = InputSettings->GetActionMappings();
        KeyBindingSettings.AxisMappings = InputSettings->GetAxisMappings();
    }

    // Apply settings
    ApplyGraphicsSettings();
    ApplyAudioSettings();
    ApplyGameplaySettings();
    ApplyKeyBindings();

    // Save settings
    SaveSettings();
}

void USettingsManager::ApplyGraphicsSettings()
{
    // Get game user settings
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (UserSettings)
    {
        // Set scalability settings based on quality preset
        if (GraphicsSettings.GraphicsQuality != EGraphicsQuality::Custom)
        {
            int32 QualityLevel = static_cast<int32>(GraphicsSettings.GraphicsQuality);
            UserSettings->SetOverallScalabilityLevel(QualityLevel);
        }
        else
        {
            // Set individual scalability settings
            UserSettings->ScalabilityQuality.SetViewDistanceQuality(GraphicsSettings.ViewDistanceQuality);
            UserSettings->ScalabilityQuality.SetAntiAliasingQuality(GraphicsSettings.AntiAliasingQuality);
            UserSettings->ScalabilityQuality.SetShadowQuality(GraphicsSettings.ShadowQuality);
            UserSettings->ScalabilityQuality.SetPostProcessQuality(GraphicsSettings.PostProcessQuality);
            UserSettings->ScalabilityQuality.SetTextureQuality(GraphicsSettings.TextureQuality);
            UserSettings->ScalabilityQuality.SetEffectsQuality(GraphicsSettings.EffectsQuality);
            UserSettings->ScalabilityQuality.SetFoliageQuality(GraphicsSettings.FoliageQuality);
        }

        // Set resolution scale
        UserSettings->SetResolutionScaleNormalized(GraphicsSettings.ResolutionScale);

        // Set VSync
        UserSettings->SetVSyncEnabled(GraphicsSettings.bVSyncEnabled);

        // Set frame rate limit
        UserSettings->SetFrameRateLimit(GraphicsSettings.FrameRateLimit);

        // Apply settings
        UserSettings->ApplySettings(false);
    }

    // Apply post-processing settings
    if (GEngine)
    {
        // Apply motion blur
        static IConsoleVariable* CVar_MotionBlur = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MotionBlur.Max"));
        if (CVar_MotionBlur)
        {
            CVar_MotionBlur->Set(GraphicsSettings.bMotionBlurEnabled ? 1 : 0);
        }

        // Apply depth of field
        static IConsoleVariable* CVar_DepthOfField = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DepthOfField.Enable"));
        if (CVar_DepthOfField)
        {
            CVar_DepthOfField->Set(GraphicsSettings.bDepthOfFieldEnabled ? 1 : 0);
        }

        // Apply ambient occlusion
        static IConsoleVariable* CVar_AmbientOcclusion = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AmbientOcclusion.Enable"));
        if (CVar_AmbientOcclusion)
        {
            CVar_AmbientOcclusion->Set(GraphicsSettings.bAmbientOcclusionEnabled ? 1 : 0);
        }

        // Apply bloom
        static IConsoleVariable* CVar_Bloom = IConsoleManager::Get().FindConsoleVariable(TEXT("r.BloomQuality"));
        if (CVar_Bloom)
        {
            CVar_Bloom->Set(GraphicsSettings.bBloomEnabled ? 1 : 0);
        }
    }
}

void USettingsManager::ApplyAudioSettings()
{
    // Apply audio settings
    USoundManager::SetMasterVolume(AudioSettings.MasterVolume);
    USoundManager::SetMusicVolume(AudioSettings.MusicVolume);
    USoundManager::SetSFXVolume(AudioSettings.SFXVolume);
    USoundManager::SetUIVolume(AudioSettings.UIVolume);

    // Apply voice chat settings
    if (GEngine)
    {
        // Apply voice chat enabled
        static IConsoleVariable* CVar_VoiceChat = IConsoleManager::Get().FindConsoleVariable(TEXT("voice.Enable"));
        if (CVar_VoiceChat)
        {
            CVar_VoiceChat->Set(AudioSettings.bVoiceChatEnabled ? 1 : 0);
        }

        // Apply push to talk
        static IConsoleVariable* CVar_PushToTalk = IConsoleManager::Get().FindConsoleVariable(TEXT("voice.PushToTalk"));
        if (CVar_PushToTalk)
        {
            CVar_PushToTalk->Set(AudioSettings.bPushToTalkEnabled ? 1 : 0);
        }
    }
}

void USettingsManager::ApplyGameplaySettings()
{
    // Apply mouse sensitivity
    if (GEngine)
    {
        // Apply mouse sensitivity
        static IConsoleVariable* CVar_MouseSensitivity = IConsoleManager::Get().FindConsoleVariable(TEXT("MouseSensitivity"));
        if (CVar_MouseSensitivity)
        {
            CVar_MouseSensitivity->Set(GameplaySettings.MouseSensitivity);
        }

        // Apply gamepad sensitivity
        static IConsoleVariable* CVar_GamepadSensitivity = IConsoleManager::Get().FindConsoleVariable(TEXT("GamepadSensitivity"));
        if (CVar_GamepadSensitivity)
        {
            CVar_GamepadSensitivity->Set(GameplaySettings.GamepadSensitivity);
        }

        // Apply invert Y axis
        static IConsoleVariable* CVar_InvertYAxis = IConsoleManager::Get().FindConsoleVariable(TEXT("InvertYAxis"));
        if (CVar_InvertYAxis)
        {
            CVar_InvertYAxis->Set(GameplaySettings.bInvertYAxis ? 1 : 0);
        }

        // Apply field of view
        static IConsoleVariable* CVar_FOV = IConsoleManager::Get().FindConsoleVariable(TEXT("FOV"));
        if (CVar_FOV)
        {
            CVar_FOV->Set(GameplaySettings.FieldOfView);
        }
    }
}

void USettingsManager::ApplyKeyBindings()
{
    // Get input settings
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();
    if (InputSettings)
    {
        // Clear existing mappings
        InputSettings->ActionMappings.Empty();
        InputSettings->AxisMappings.Empty();

        // Add action mappings
        for (const FInputActionKeyMapping& Mapping : KeyBindingSettings.ActionMappings)
        {
            InputSettings->AddActionMapping(Mapping);
        }

        // Add axis mappings
        for (const FInputAxisKeyMapping& Mapping : KeyBindingSettings.AxisMappings)
        {
            InputSettings->AddAxisMapping(Mapping);
        }

        // Save input settings
        InputSettings->SaveKeyMappings();
        InputSettings->ForceRebuildKeymaps();
    }
}

void USettingsManager::SetGraphicsSettings(const FGraphicsSettings& NewSettings)
{
    GraphicsSettings = NewSettings;
    ApplyGraphicsSettings();
    SaveSettings();
}

void USettingsManager::SetAudioSettings(const FAudioSettings& NewSettings)
{
    AudioSettings = NewSettings;
    ApplyAudioSettings();
    SaveSettings();
}

void USettingsManager::SetGameplaySettings(const FGameplaySettings& NewSettings)
{
    GameplaySettings = NewSettings;
    ApplyGameplaySettings();
    SaveSettings();
}

void USettingsManager::SetKeyBindingSettings(const FKeyBindingSettings& NewSettings)
{
    KeyBindingSettings = NewSettings;
    ApplyKeyBindings();
    SaveSettings();
}

USettingsManager* USettingsManager::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<USettingsManager>();
        Instance->Initialize();
        Instance->AddToRoot(); // Prevent garbage collection
    }

    return Instance;
}
