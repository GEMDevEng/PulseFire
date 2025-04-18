#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../PulseFireGameInstance.h"

UServerRowWidget::UServerRowWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UServerRowWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnJoinButtonClicked);
    }
}

void UServerRowWidget::SetSessionResult(const FOnlineSessionSearchResult& InSessionResult)
{
    SessionResult = InSessionResult;
    UpdateWidget();
}

void UServerRowWidget::OnJoinButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Join the session
        GameInstance->JoinSession(SessionResult);
    }
}

void UServerRowWidget::UpdateWidget()
{
    // Get session properties
    FString ServerName = "Unknown";
    int32 CurrentPlayers = 0;
    int32 MaxPlayers = 0;

    // Get server name
    SessionResult.Session.SessionSettings.Get(TEXT("SERVER_NAME"), ServerName);

    // Get player counts
    CurrentPlayers = SessionResult.Session.SessionSettings.NumPublicConnections - SessionResult.Session.NumOpenPublicConnections;
    MaxPlayers = SessionResult.Session.SessionSettings.NumPublicConnections;

    // Update server name
    if (ServerNameText)
    {
        ServerNameText->SetText(FText::FromString(ServerName));
    }

    // Update ping
    if (PingText)
    {
        FString PingString = FString::Printf(TEXT("%d ms"), SessionResult.PingInMs);
        PingText->SetText(FText::FromString(PingString));
    }

    // Update players
    if (PlayersText)
    {
        FString PlayersString = FString::Printf(TEXT("%d/%d"), CurrentPlayers, MaxPlayers);
        PlayersText->SetText(FText::FromString(PlayersString));
    }
}
