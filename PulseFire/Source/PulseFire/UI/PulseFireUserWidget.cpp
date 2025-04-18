#include "PulseFireUserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

UPulseFireUserWidget::UPulseFireUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVisible = false;
}

void UPulseFireUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPulseFireUserWidget::NativeOnAddedToViewport()
{
    Super::NativeOnAddedToViewport();
    bIsVisible = true;
}

void UPulseFireUserWidget::NativeOnRemovedFromViewport()
{
    Super::NativeOnRemovedFromViewport();
    bIsVisible = false;
}

void UPulseFireUserWidget::ShowWithAnimation()
{
    if (!bIsVisible)
    {
        AddToViewport();
        
        if (ShowAnimation)
        {
            PlayAnimation(ShowAnimation);
        }
    }
}

void UPulseFireUserWidget::HideWithAnimation()
{
    if (bIsVisible)
    {
        if (HideAnimation)
        {
            PlayAnimation(HideAnimation);
            
            // Set a timer to remove the widget after the animation completes
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                [this]() { RemoveFromParent(); },
                HideAnimation->GetEndTime(),
                false
            );
        }
        else
        {
            RemoveFromParent();
        }
    }
}

void UPulseFireUserWidget::SetOwningPlayer(APlayerController* NewOwningPlayer)
{
    if (NewOwningPlayer)
    {
        SetOwningPlayer(NewOwningPlayer);
    }
}

void UPulseFireUserWidget::PlayAnimation(UWidgetAnimation* InAnimation, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
    if (InAnimation)
    {
        Super::PlayAnimation(InAnimation, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
    }
}
