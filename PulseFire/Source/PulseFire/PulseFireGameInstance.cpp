#include "PulseFireGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UPulseFireGameInstance::UPulseFireGameInstance()
{
	// Set default values
	SessionName = TEXT("PulseFire Game Session");

	// Bind delegates
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPulseFireGameInstance::OnCreateSessionComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPulseFireGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPulseFireGameInstance::OnDestroySessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPulseFireGameInstance::OnFindSessionsComplete);
}

void UPulseFireGameInstance::Init()
{
	Super::Init();

	// Get the online subsystem
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		// Get the session interface
		SessionInterface = Subsystem->GetSessionInterface();
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No subsystem found"));
	}
}

void UPulseFireGameInstance::HostSession(FString ServerName, bool bIsLAN, int32 MaxPlayers)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	// Check if a session already exists
	auto ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession)
	{
		// Destroy the existing session
		SessionInterface->DestroySession(SessionName);
	}

	// Create session settings
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = bIsLAN;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->NumPublicConnections = MaxPlayers;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bIsDedicated = false;
	SessionSettings->bAllowInvites = true;

	// Set session properties
	SessionSettings->Set(TEXT("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(TEXT("GAME_VERSION"), FString("1.0"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Register delegate and create session
	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	if (!SessionInterface->CreateSession(0, SessionName, *SessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session"));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UPulseFireGameInstance::JoinSessionByIP(FString IPAddress)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
		OnJoinSessionCompleteEvent.Broadcast(false);
		return;
	}

	// Check if a session already exists
	auto ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession)
	{
		// Destroy the existing session
		SessionInterface->DestroySession(SessionName);
	}

	// Join the session using IP address
	FOnlineSessionSearchResult SearchResult;
	SearchResult.Session.OwningUserId = FUniqueNetIdString::Create(IPAddress, TEXT("NULL"));
	SearchResult.PingInMs = 0;

	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	if (!SessionInterface->JoinSession(0, SessionName, SearchResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session by IP"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		OnJoinSessionCompleteEvent.Broadcast(false);
	}
}

void UPulseFireGameInstance::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
		OnJoinSessionCompleteEvent.Broadcast(false);
		return;
	}

	// Check if a session already exists
	auto ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession)
	{
		// Destroy the existing session
		SessionInterface->DestroySession(SessionName);
	}

	// Join the session
	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	if (!SessionInterface->JoinSession(0, SessionName, SessionResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session from search result"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		OnJoinSessionCompleteEvent.Broadcast(false);
	}
}

void UPulseFireGameInstance::FindSessions(bool bIsLAN)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
		return;
	}

	// Create session search object
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = bIsLAN;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->PingBucketSize = 50;
	SessionSearch->TimeoutInSeconds = 15;

	// Set search parameters
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// Register delegate and start search
	OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	if (!SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find sessions"));
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	}
}

void UPulseFireGameInstance::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	// Check if a session exists
	auto ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession)
	{
		// Register delegate and destroy session
		OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
		if (!SessionInterface->DestroySession(SessionName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
			SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			OnDestroySessionCompleteEvent.Broadcast(false);
		}
	}
	else
	{
		// No session to destroy, just broadcast success
		OnDestroySessionCompleteEvent.Broadcast(true);
	}
}

void UPulseFireGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete: %s, %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));

	// Clear delegate
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	// Broadcast result
	OnCreateSessionCompleteEvent.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		// Travel to the game map
		TravelToGameMap();
	}
}

void UPulseFireGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete: %s, %d"), *SessionName.ToString(), static_cast<int32>(Result));

	// Clear delegate
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	// Broadcast result
	bool bWasSuccessful = (Result == EOnJoinSessionCompleteResult::Success);
	OnJoinSessionCompleteEvent.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		// Get the connection string
		FString ConnectString;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
		{
			// Travel to the session
			APlayerController* PlayerController = GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UPulseFireGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete: %s, %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));

	// Clear delegate
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}

	// Broadcast result
	OnDestroySessionCompleteEvent.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		// Return to main menu
		TravelToMainMenu();
	}
}

void UPulseFireGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete: %s"), bWasSuccessful ? TEXT("true") : TEXT("false"));

	// Clear delegate
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	}

	if (bWasSuccessful && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());

		// Broadcast the search results
		OnSessionsFoundEvent.Broadcast(SessionSearch->SearchResults);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search failed"));
		OnSessionsFoundEvent.Broadcast(TArray<FOnlineSessionSearchResult>());
	}
}

void UPulseFireGameInstance::TravelToGameMap()
{
	// Travel to the game map
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap"), true, "listen");
}

void UPulseFireGameInstance::TravelToMainMenu()
{
	// Return to main menu
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"), true);
}
