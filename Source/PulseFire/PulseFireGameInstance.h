#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PulseFireGameInstance.generated.h"

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
	void HostSession(FString ServerName, bool bIsLAN);

	/** Join an existing game session */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinSession(FString IPAddress);

	/** Destroy the current session and return to main menu */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void DestroySession();

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

	/** Delegate handle for creating a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/** Delegate handle for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/** Delegate handle for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/** Callback for when session creation is complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/** Callback for when joining a session is complete */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/** Callback for when destroying a session is complete */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
