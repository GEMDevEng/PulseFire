#pragma once

#include "CoreMinimal.h"
#include "PulseFireUserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UEditableTextBox;
class UScrollBox;
class UServerRowWidget;

/**
 * Widget for multiplayer menu.
 */
UCLASS()
class PULSEFIRE_API UMultiplayerMenuWidget : public UPulseFireUserWidget
{
    GENERATED_BODY()
    
public:
    UMultiplayerMenuWidget(const FObjectInitializer& ObjectInitializer);

    /** Initialize the widget */
    virtual void NativeConstruct() override;

    /** Called when the widget is added to the viewport */
    virtual void NativeOnAddedToViewport() override;

    /** Called when the widget is removed from the viewport */
    virtual void NativeOnRemovedFromViewport() override;

    /** Update the server list */
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void UpdateServerList(const TArray<FOnlineSessionSearchResult>& SearchResults);

protected:
    /** Host game button */
    UPROPERTY(meta = (BindWidget))
    UButton* HostGameButton;

    /** Join IP button */
    UPROPERTY(meta = (BindWidget))
    UButton* JoinIPButton;

    /** Refresh button */
    UPROPERTY(meta = (BindWidget))
    UButton* RefreshButton;

    /** Back button */
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    /** IP address text box */
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* IPAddressTextBox;

    /** Server list scroll box */
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ServerListScrollBox;

    /** Server row widget class */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UServerRowWidget> ServerRowWidgetClass;

    /** Host game button clicked */
    UFUNCTION()
    void OnHostGameButtonClicked();

    /** Join IP button clicked */
    UFUNCTION()
    void OnJoinIPButtonClicked();

    /** Refresh button clicked */
    UFUNCTION()
    void OnRefreshButtonClicked();

    /** Back button clicked */
    UFUNCTION()
    void OnBackButtonClicked();

    /** Sessions found callback */
    UFUNCTION()
    void OnSessionsFound(const TArray<FOnlineSessionSearchResult>& SessionResults);

    /** Create session complete callback */
    UFUNCTION()
    void OnCreateSessionComplete(bool Successful);

    /** Join session complete callback */
    UFUNCTION()
    void OnJoinSessionComplete(bool Successful);
};
