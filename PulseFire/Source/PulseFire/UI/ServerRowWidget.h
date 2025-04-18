#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerRowWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * Widget for displaying a server in the server list.
 */
UCLASS()
class PULSEFIRE_API UServerRowWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UServerRowWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Set the session result */
    UFUNCTION(BlueprintCallable, Category = "Server")
    void SetSessionResult(const FOnlineSessionSearchResult& InSessionResult);

protected:
    /** Server name text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ServerNameText;

    /** Ping text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PingText;

    /** Players text */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayersText;

    /** Join button */
    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    /** Session result */
    FOnlineSessionSearchResult SessionResult;

    /** Join button clicked */
    UFUNCTION()
    void OnJoinButtonClicked();

    /** Update the widget */
    void UpdateWidget();
};
