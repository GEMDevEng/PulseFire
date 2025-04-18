#include "AnalyticsManager.h"
#include "Misc/Guid.h"
#include "Misc/App.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformMisc.h"

// Initialize static instance
UAnalyticsManager* UAnalyticsManager::Instance = nullptr;

UAnalyticsManager::UAnalyticsManager()
{
    // Set default values
    bAnalyticsEnabled = true;
    UserID = "";
    SessionID = "";
}

void UAnalyticsManager::Initialize()
{
    // Load analytics settings from config
    bool bConfigAnalyticsEnabled = true;
    GConfig->GetBool(TEXT("Analytics"), TEXT("bEnabled"), bConfigAnalyticsEnabled, GGameIni);
    bAnalyticsEnabled = bConfigAnalyticsEnabled;

    // Load or generate user ID
    FString ConfigUserID;
    if (GConfig->GetString(TEXT("Analytics"), TEXT("UserID"), ConfigUserID, GGameIni) && !ConfigUserID.IsEmpty())
    {
        UserID = ConfigUserID;
    }
    else
    {
        // Generate a new user ID
        UserID = GenerateUniqueID();
        
        // Save to config
        GConfig->SetString(TEXT("Analytics"), TEXT("UserID"), *UserID, GGameIni);
        GConfig->Flush(false, GGameIni);
    }

    // Generate a new session ID
    SessionID = GenerateUniqueID();

    // Track session start
    TrackEvent("SessionStart");

    // Set default user properties
    SetUserProperty("Platform", FString(FPlatformProperties::PlatformName()));
    SetUserProperty("OS", FPlatformMisc::GetOSVersion());
    SetUserProperty("CPU", FPlatformMisc::GetCPUBrand());
    SetUserProperty("GPU", FPlatformMisc::GetPrimaryGPUBrand());
    SetUserProperty("RAM", FString::Printf(TEXT("%.1f GB"), FPlatformMemory::GetPhysicalGBRam()));
    SetUserProperty("AppVersion", FApp::GetProjectVersion());
}

void UAnalyticsManager::TrackEvent(const FString& EventName)
{
    if (!bAnalyticsEnabled)
    {
        return;
    }

    // Create event
    FAnalyticsEvent Event(EventName);
    
    // Add to queue
    EventQueue.Add(Event);
    
    // Flush if queue is large enough
    if (EventQueue.Num() >= 10)
    {
        FlushEvents();
    }
}

void UAnalyticsManager::TrackEventWithParams(const FString& EventName, const TMap<FString, FString>& Parameters)
{
    if (!bAnalyticsEnabled)
    {
        return;
    }

    // Create event
    FAnalyticsEvent Event(EventName);
    Event.Parameters = Parameters;
    
    // Add to queue
    EventQueue.Add(Event);
    
    // Flush if queue is large enough
    if (EventQueue.Num() >= 10)
    {
        FlushEvents();
    }
}

void UAnalyticsManager::TrackScreenView(const FString& ScreenName)
{
    if (!bAnalyticsEnabled)
    {
        return;
    }

    // Create event
    FAnalyticsEvent Event("ScreenView");
    Event.Parameters.Add("ScreenName", ScreenName);
    
    // Add to queue
    EventQueue.Add(Event);
}

void UAnalyticsManager::SetUserProperty(const FString& PropertyName, const FString& PropertyValue)
{
    if (!bAnalyticsEnabled)
    {
        return;
    }

    // Set user property
    UserProperties.Add(PropertyName, PropertyValue);
}

void UAnalyticsManager::SubmitFeedback(const FString& FeedbackText, int32 Rating, const FString& Category)
{
    if (!bAnalyticsEnabled)
    {
        return;
    }

    // Create event
    FAnalyticsEvent Event("UserFeedback");
    Event.Parameters.Add("Feedback", FeedbackText);
    Event.Parameters.Add("Rating", FString::FromInt(Rating));
    Event.Parameters.Add("Category", Category);
    
    // Add to queue and flush immediately
    EventQueue.Add(Event);
    FlushEvents();
}

void UAnalyticsManager::SubmitCrashReport(const FString& CrashLog, const FString& AdditionalInfo)
{
    // Always submit crash reports, even if analytics is disabled
    
    // Create event
    FAnalyticsEvent Event("CrashReport");
    Event.Parameters.Add("CrashLog", CrashLog);
    Event.Parameters.Add("AdditionalInfo", AdditionalInfo);
    
    // Add to queue and flush immediately
    EventQueue.Add(Event);
    FlushEvents();
}

void UAnalyticsManager::EnableAnalytics(bool bEnable)
{
    bAnalyticsEnabled = bEnable;
    
    // Save to config
    GConfig->SetBool(TEXT("Analytics"), TEXT("bEnabled"), bAnalyticsEnabled, GGameIni);
    GConfig->Flush(false, GGameIni);
    
    // Track analytics opt-in/opt-out
    if (bAnalyticsEnabled)
    {
        TrackEvent("AnalyticsOptIn");
    }
    else
    {
        TrackEvent("AnalyticsOptOut");
        FlushEvents();
    }
}

bool UAnalyticsManager::IsAnalyticsEnabled() const
{
    return bAnalyticsEnabled;
}

UAnalyticsManager* UAnalyticsManager::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UAnalyticsManager>();
        Instance->Initialize();
        Instance->AddToRoot(); // Prevent garbage collection
    }
    
    return Instance;
}

void UAnalyticsManager::FlushEvents()
{
    if (EventQueue.Num() == 0)
    {
        return;
    }

    // In a real implementation, this would send events to an analytics server
    // For now, we'll just log them to a file
    
    FString LogFilePath = FPaths::ProjectSavedDir() / TEXT("Analytics") / TEXT("analytics.log");
    
    // Create directory if it doesn't exist
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.CreateDirectoryTree(*FPaths::GetPath(LogFilePath));
    
    // Format events as JSON
    FString EventsJson = TEXT("[\n");
    
    for (int32 i = 0; i < EventQueue.Num(); i++)
    {
        const FAnalyticsEvent& Event = EventQueue[i];
        
        FString EventJson = FString::Printf(TEXT("  {\n    \"event\": \"%s\",\n    \"timestamp\": \"%s\",\n    \"user_id\": \"%s\",\n    \"session_id\": \"%s\",\n"),
            *Event.EventName,
            *Event.Timestamp.ToString(),
            *UserID,
            *SessionID);
        
        // Add parameters
        EventJson += TEXT("    \"parameters\": {\n");
        
        int32 ParamIndex = 0;
        for (const auto& Param : Event.Parameters)
        {
            EventJson += FString::Printf(TEXT("      \"%s\": \"%s\"%s\n"),
                *Param.Key,
                *Param.Value,
                (ParamIndex < Event.Parameters.Num() - 1) ? TEXT(",") : TEXT(""));
            
            ParamIndex++;
        }
        
        EventJson += TEXT("    }\n  }");
        
        if (i < EventQueue.Num() - 1)
        {
            EventJson += TEXT(",");
        }
        
        EventJson += TEXT("\n");
        
        EventsJson += EventJson;
    }
    
    EventsJson += TEXT("]\n");
    
    // Append to log file
    FFileHelper::SaveStringToFile(EventsJson, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
    
    // Clear queue
    EventQueue.Empty();
}

FString UAnalyticsManager::GenerateUniqueID() const
{
    return FGuid::NewGuid().ToString();
}
