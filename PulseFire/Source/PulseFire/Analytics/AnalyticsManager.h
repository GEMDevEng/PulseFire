#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AnalyticsManager.generated.h"

USTRUCT(BlueprintType)
struct FAnalyticsEvent
{
    GENERATED_BODY()

    /** Event name */
    UPROPERTY(BlueprintReadWrite, Category = "Analytics")
    FString EventName;

    /** Event parameters */
    UPROPERTY(BlueprintReadWrite, Category = "Analytics")
    TMap<FString, FString> Parameters;

    /** Event timestamp */
    UPROPERTY(BlueprintReadWrite, Category = "Analytics")
    FDateTime Timestamp;

    FAnalyticsEvent()
    {
        Timestamp = FDateTime::UtcNow();
    }

    FAnalyticsEvent(const FString& InEventName)
        : EventName(InEventName)
    {
        Timestamp = FDateTime::UtcNow();
    }
};

/**
 * Manager for tracking analytics and user feedback.
 */
UCLASS(Blueprintable, BlueprintType)
class PULSEFIRE_API UAnalyticsManager : public UObject
{
    GENERATED_BODY()
    
public:
    UAnalyticsManager();

    /** Initialize the analytics manager */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void Initialize();

    /** Track an event */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void TrackEvent(const FString& EventName);

    /** Track an event with parameters */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void TrackEventWithParams(const FString& EventName, const TMap<FString, FString>& Parameters);

    /** Track a screen view */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void TrackScreenView(const FString& ScreenName);

    /** Track a user property */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void SetUserProperty(const FString& PropertyName, const FString& PropertyValue);

    /** Submit user feedback */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void SubmitFeedback(const FString& FeedbackText, int32 Rating, const FString& Category);

    /** Submit a crash report */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void SubmitCrashReport(const FString& CrashLog, const FString& AdditionalInfo);

    /** Enable analytics collection */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void EnableAnalytics(bool bEnable);

    /** Check if analytics collection is enabled */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    bool IsAnalyticsEnabled() const;

    /** Get the singleton instance */
    UFUNCTION(BlueprintCallable, Category = "Analytics")
    static UAnalyticsManager* GetInstance();

private:
    /** Whether analytics collection is enabled */
    bool bAnalyticsEnabled;

    /** User ID */
    FString UserID;

    /** Session ID */
    FString SessionID;

    /** Event queue */
    TArray<FAnalyticsEvent> EventQueue;

    /** User properties */
    TMap<FString, FString> UserProperties;

    /** Flush events to the server */
    void FlushEvents();

    /** Generate a unique ID */
    FString GenerateUniqueID() const;

    /** Singleton instance */
    static UAnalyticsManager* Instance;
};
