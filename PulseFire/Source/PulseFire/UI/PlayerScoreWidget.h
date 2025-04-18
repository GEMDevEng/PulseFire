#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "PlayerScoreWidget.generated.h"

class UTextBlock;
class APulseFirePlayerState;

/**
 * Widget for displaying player score in match results.
 */
UCLASS()
class PULSEFIRE_API UPlayerScoreWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UPlayerScoreWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Set the player state */
    UFUNCTION(BlueprintCallable, Category = "Player Score")
    void SetPlayerState(APulseFirePlayerState* InPlayerState);

protected:
    /** Player name text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText;

    /** Score text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ScoreText;

    /** Kills text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsText;

    /** Deaths text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsText;

    /** K/D ratio text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* KDRatioText;

    /** Player state */
    UPROPERTY()
    APulseFirePlayerState* PlayerState;

    /** Update the widget */
    void UpdateWidget();
};
