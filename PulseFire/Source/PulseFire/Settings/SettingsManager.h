#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/PlayerInput.h"
#include "SettingsManager.generated.h"

UENUM(BlueprintType)
enum class EGraphicsQuality : uint8
{
    Low         UMETA(DisplayName = "Low"),
    Medium      UMETA(DisplayName = "Medium"),
    High        UMETA(DisplayName = "High"),
    Epic        UMETA(DisplayName = "Epic"),
    Custom      UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FGraphicsSettings
{
    GENERATED_BODY()

    /** Overall graphics quality */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    EGraphicsQuality GraphicsQuality;

    /** Resolution scale (0.5 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0.5", ClampMax = "1.0"))
    float ResolutionScale;

    /** View distance quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 ViewDistanceQuality;

    /** Anti-aliasing quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 AntiAliasingQuality;

    /** Shadow quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 ShadowQuality;

    /** Post-processing quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 PostProcessQuality;

    /** Texture quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 TextureQuality;

    /** Effects quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 EffectsQuality;

    /** Foliage quality (0-3) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "3"))
    int32 FoliageQuality;

    /** Vertical sync enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    bool bVSyncEnabled;

    /** Frame rate limit (0 = unlimited) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics", meta = (ClampMin = "0", ClampMax = "240"))
    int32 FrameRateLimit;

    /** Motion blur enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    bool bMotionBlurEnabled;

    /** Depth of field enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    bool bDepthOfFieldEnabled;

    /** Ambient occlusion enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    bool bAmbientOcclusionEnabled;

    /** Bloom enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    bool bBloomEnabled;

    FGraphicsSettings()
    {
        GraphicsQuality = EGraphicsQuality::Medium;
        ResolutionScale = 1.0f;
        ViewDistanceQuality = 2;
        AntiAliasingQuality = 2;
        ShadowQuality = 2;
        PostProcessQuality = 2;
        TextureQuality = 2;
        EffectsQuality = 2;
        FoliageQuality = 2;
        bVSyncEnabled = false;
        FrameRateLimit = 144;
        bMotionBlurEnabled = true;
        bDepthOfFieldEnabled = true;
        bAmbientOcclusionEnabled = true;
        bBloomEnabled = true;
    }
};

USTRUCT(BlueprintType)
struct FAudioSettings
{
    GENERATED_BODY()

    /** Master volume (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MasterVolume;

    /** Music volume (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MusicVolume;

    /** SFX volume (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SFXVolume;

    /** UI volume (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float UIVolume;

    /** Voice chat volume (0.0 - 1.0) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VoiceChatVolume;

    /** Voice chat enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bVoiceChatEnabled;

    /** Push to talk enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bPushToTalkEnabled;

    /** Mute when minimized */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    bool bMuteWhenMinimized;

    FAudioSettings()
    {
        MasterVolume = 1.0f;
        MusicVolume = 0.8f;
        SFXVolume = 1.0f;
        UIVolume = 0.8f;
        VoiceChatVolume = 1.0f;
        bVoiceChatEnabled = true;
        bPushToTalkEnabled = true;
        bMuteWhenMinimized = true;
    }
};

USTRUCT(BlueprintType)
struct FGameplaySettings
{
    GENERATED_BODY()

    /** Mouse sensitivity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float MouseSensitivity;

    /** Gamepad sensitivity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float GamepadSensitivity;

    /** Invert Y axis */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bInvertYAxis;

    /** Auto-reload */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bAutoReload;

    /** Toggle aim */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bToggleAim;

    /** Toggle crouch */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bToggleCrouch;

    /** Toggle sprint */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bToggleSprint;

    /** Show damage numbers */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bShowDamageNumbers;

    /** Show hit markers */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bShowHitMarkers;

    /** Show crosshair */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    bool bShowCrosshair;

    /** Field of view (60-120) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "60", ClampMax = "120"))
    float FieldOfView;

    FGameplaySettings()
    {
        MouseSensitivity = 1.0f;
        GamepadSensitivity = 1.0f;
        bInvertYAxis = false;
        bAutoReload = true;
        bToggleAim = false;
        bToggleCrouch = false;
        bToggleSprint = false;
        bShowDamageNumbers = true;
        bShowHitMarkers = true;
        bShowCrosshair = true;
        FieldOfView = 90.0f;
    }
};

USTRUCT(BlueprintType)
struct FKeyBindingSettings
{
    GENERATED_BODY()

    /** Key bindings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TArray<FInputActionKeyMapping> ActionMappings;

    /** Axis bindings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TArray<FInputAxisKeyMapping> AxisMappings;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsChanged);

/**
 * Manager for handling game settings.
 */
UCLASS(Blueprintable, BlueprintType)
class PULSEFIRE_API USettingsManager : public UObject
{
    GENERATED_BODY()
    
public:
    USettingsManager();

    /** Initialize the settings manager */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void Initialize();

    /** Save settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SaveSettings();

    /** Load settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void LoadSettings();

    /** Reset settings to defaults */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ResetToDefaults();

    /** Apply graphics settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyGraphicsSettings();

    /** Apply audio settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyAudioSettings();

    /** Apply gameplay settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyGameplaySettings();

    /** Apply key bindings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyKeyBindings();

    /** Get graphics settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FGraphicsSettings GetGraphicsSettings() const { return GraphicsSettings; }

    /** Set graphics settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetGraphicsSettings(const FGraphicsSettings& NewSettings);

    /** Get audio settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FAudioSettings GetAudioSettings() const { return AudioSettings; }

    /** Set audio settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetAudioSettings(const FAudioSettings& NewSettings);

    /** Get gameplay settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FGameplaySettings GetGameplaySettings() const { return GameplaySettings; }

    /** Set gameplay settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetGameplaySettings(const FGameplaySettings& NewSettings);

    /** Get key binding settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    FKeyBindingSettings GetKeyBindingSettings() const { return KeyBindingSettings; }

    /** Set key binding settings */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetKeyBindingSettings(const FKeyBindingSettings& NewSettings);

    /** Get the singleton instance */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    static USettingsManager* GetInstance();

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Settings")
    FOnSettingsChanged OnSettingsChanged;

private:
    /** Graphics settings */
    UPROPERTY()
    FGraphicsSettings GraphicsSettings;

    /** Audio settings */
    UPROPERTY()
    FAudioSettings AudioSettings;

    /** Gameplay settings */
    UPROPERTY()
    FGameplaySettings GameplaySettings;

    /** Key binding settings */
    UPROPERTY()
    FKeyBindingSettings KeyBindingSettings;

    /** Singleton instance */
    static USettingsManager* Instance;
};
