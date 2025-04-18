#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PulseFireUserWidget.generated.h"

/**
 * Base class for all PulseFire UI widgets.
 * Provides common functionality for UI widgets.
 */
UCLASS()
class PULSEFIRE_API UPulseFireUserWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPulseFireUserWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Called when the widget is added to the viewport */
    virtual void NativeOnAddedToViewport() override;

    /** Called when the widget is removed from the viewport */
    virtual void NativeOnRemovedFromViewport() override;

    /** Show the widget with animation */
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowWithAnimation();

    /** Hide the widget with animation */
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void HideWithAnimation();

    /** Set the owner of the widget */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetOwningPlayer(APlayerController* NewOwningPlayer);

protected:
    /** Animation to play when showing the widget */
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ShowAnimation;

    /** Animation to play when hiding the widget */
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HideAnimation;

    /** Whether the widget is currently visible */
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    bool bIsVisible;

    /** Play animation */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayAnimation(UWidgetAnimation* InAnimation, float StartAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1.0f);
};
