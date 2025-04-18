#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "PulseFireGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, const TArray<FOnlineSessionSearchResult>&, SessionResults);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionComplete, bool, Successful);

/**
 * Game instance class for PulseFire.
 * Handles session management and network functionality.
 */
UCLASS()
class PULSEFIRE_API UPulseFireGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPulseFireGameInstance();

	/** Initialize the game instance */
	virtual void Init() override;

	/** Host a new game session */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void HostSession(FString ServerName, bool bIsLAN, int32 MaxPlayers = 4);

	/** Join an existing game session by IP address */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinSessionByIP(FString IPAddress);

	/** Join an existing game session by search result */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	/** Find available sessions */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void FindSessions(bool bIsLAN);

	/** Destroy the current session and return to main menu */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void DestroySession();

	/** Get the current session name */
	UFUNCTION(BlueprintCallable, Category = "Network")
	FName GetSessionName() const { return SessionName; }

	/** Get the current session interface */
	IOnlineSessionPtr GetSessionInterface() const { return SessionInterface; }

	/** Events for session operations */
	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnSessionsFound OnSessionsFoundEvent;

	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnCreateSessionComplete OnCreateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnJoinSessionComplete OnJoinSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnDestroySessionComplete OnDestroySessionCompleteEvent;

protected:
	/** Pointer to the online session interface */
	IOnlineSessionPtr SessionInterface;

	/** Name of the current session */
	FName SessionName;

	/** Delegate for creating a session */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Delegate for finding sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Delegate handle for creating a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/** Delegate handle for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/** Delegate handle for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/** Delegate handle for finding sessions */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/** Session search object */
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	/** Callback for when session creation is complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/** Callback for when joining a session is complete */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/** Callback for when destroying a session is complete */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/** Callback for when finding sessions is complete */
	void OnFindSessionsComplete(bool bWasSuccessful);

	/** Travel to the game map */
	void TravelToGameMap();

	/** Travel to the main menu */
	void TravelToMainMenu();
};
