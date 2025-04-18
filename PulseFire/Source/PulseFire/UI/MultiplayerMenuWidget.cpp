#include "MultiplayerMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "../PulseFireGameInstance.h"
#include "ServerRowWidget.h"

UMultiplayerMenuWidget::UMultiplayerMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMultiplayerMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (HostGameButton)
    {
        HostGameButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnHostGameButtonClicked);
    }

    if (JoinIPButton)
    {
        JoinIPButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnJoinIPButtonClicked);
    }

    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnRefreshButtonClicked);
    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnBackButtonClicked);
    }

    // Set default IP address
    if (IPAddressTextBox)
    {
        IPAddressTextBox->SetText(FText::FromString("127.0.0.1"));
    }
}

void UMultiplayerMenuWidget::NativeOnAddedToViewport()
{
    Super::NativeOnAddedToViewport();

    // Register delegates
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->OnSessionsFoundEvent.AddDynamic(this, &UMultiplayerMenuWidget::OnSessionsFound);
        GameInstance->OnCreateSessionCompleteEvent.AddDynamic(this, &UMultiplayerMenuWidget::OnCreateSessionComplete);
        GameInstance->OnJoinSessionCompleteEvent.AddDynamic(this, &UMultiplayerMenuWidget::OnJoinSessionComplete);
    }

    // Refresh server list
    OnRefreshButtonClicked();
}

void UMultiplayerMenuWidget::NativeOnRemovedFromViewport()
{
    Super::NativeOnRemovedFromViewport();

    // Unregister delegates
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->OnSessionsFoundEvent.RemoveDynamic(this, &UMultiplayerMenuWidget::OnSessionsFound);
        GameInstance->OnCreateSessionCompleteEvent.RemoveDynamic(this, &UMultiplayerMenuWidget::OnCreateSessionComplete);
        GameInstance->OnJoinSessionCompleteEvent.RemoveDynamic(this, &UMultiplayerMenuWidget::OnJoinSessionComplete);
    }
}

void UMultiplayerMenuWidget::UpdateServerList(const TArray<FOnlineSessionSearchResult>& SearchResults)
{
    // Clear server list
    if (ServerListScrollBox)
    {
        ServerListScrollBox->ClearChildren();

        // Add server rows
        for (const FOnlineSessionSearchResult& SearchResult : SearchResults)
        {
            UServerRowWidget* ServerRowWidget = CreateWidget<UServerRowWidget>(GetOwningPlayer(), ServerRowWidgetClass);
            if (ServerRowWidget)
            {
                ServerRowWidget->SetSessionResult(SearchResult);
                ServerListScrollBox->AddChild(ServerRowWidget);
            }
        }
    }
}

void UMultiplayerMenuWidget::OnHostGameButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Host a game
        GameInstance->HostSession("PulseFire Game", false);
    }
}

void UMultiplayerMenuWidget::OnJoinIPButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance && IPAddressTextBox)
    {
        // Join a game by IP
        FString IPAddress = IPAddressTextBox->GetText().ToString();
        GameInstance->JoinSessionByIP(IPAddress);
    }
}

void UMultiplayerMenuWidget::OnRefreshButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Find sessions
        GameInstance->FindSessions(false);
    }
}

void UMultiplayerMenuWidget::OnBackButtonClicked()
{
    // Hide the widget
    HideWithAnimation();
}

void UMultiplayerMenuWidget::OnSessionsFound(const TArray<FOnlineSessionSearchResult>& SessionResults)
{
    // Update server list
    UpdateServerList(SessionResults);
}

void UMultiplayerMenuWidget::OnCreateSessionComplete(bool Successful)
{
    if (Successful)
    {
        // Hide the widget
        HideWithAnimation();
    }
}

void UMultiplayerMenuWidget::OnJoinSessionComplete(bool Successful)
{
    if (Successful)
    {
        // Hide the widget
        HideWithAnimation();
    }
}
