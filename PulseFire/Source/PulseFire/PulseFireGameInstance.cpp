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

void UPulseFireGameInstance::HostSession(FString ServerName, bool bIsLAN)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
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
	SessionSettings->bUsesPresence = false;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(TEXT("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Register delegate and create session
	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	if (!SessionInterface->CreateSession(0, SessionName, *SessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session"));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}
}

void UPulseFireGameInstance::JoinSession(FString IPAddress)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
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
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}
}

void UPulseFireGameInstance::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid"));
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
		}
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

	if (bWasSuccessful)
	{
		// Travel to the game map
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap"), true, "listen");
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

	if (Result == EOnJoinSessionCompleteResult::Success)
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

	if (bWasSuccessful)
	{
		// Return to main menu
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"), true);
	}
}
