#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

class UUserWidget;
class UTutorialWidget;

USTRUCT(BlueprintType)
struct FTutorialStep
{
    GENERATED_BODY()

    /** Tutorial step ID */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName StepID;

    /** Tutorial step title */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Title;

    /** Tutorial step description */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Description;

    /** Tutorial step image */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture2D* Image;

    /** Tutorial step duration (0 = manual advance) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Duration;

    /** Whether this step requires input to advance */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bRequiresInput;

    /** Input action name required to advance */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName RequiredInputAction;

    /** Whether this step has been completed */
    UPROPERTY(BlueprintReadOnly)
    bool bCompleted;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialStepChanged, const FTutorialStep&, NewStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialCompleted);

/**
 * Manager for handling in-game tutorials.
 */
UCLASS()
class PULSEFIRE_API ATutorialManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ATutorialManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Tutorial widget class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial")
    TSubclassOf<UTutorialWidget> TutorialWidgetClass;

    /** Tutorial widget instance */
    UPROPERTY(BlueprintReadOnly, Category = "Tutorial")
    UTutorialWidget* TutorialWidget;

    /** Tutorial steps */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial")
    TArray<FTutorialStep> TutorialSteps;

    /** Current tutorial step index */
    UPROPERTY(BlueprintReadOnly, Category = "Tutorial")
    int32 CurrentStepIndex;

    /** Whether the tutorial is active */
    UPROPERTY(BlueprintReadOnly, Category = "Tutorial")
    bool bTutorialActive;

    /** Whether the tutorial is paused */
    UPROPERTY(BlueprintReadOnly, Category = "Tutorial")
    bool bTutorialPaused;

    /** Timer handle for auto-advancing tutorial steps */
    FTimerHandle TimerHandle_NextStep;

    /** Advance to the next tutorial step */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void NextStep();

    /** Go back to the previous tutorial step */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void PreviousStep();

    /** Show the current tutorial step */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ShowCurrentStep();

    /** Hide the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void HideTutorial();

    /** Complete the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void CompleteTutorial();

    /** Handle input action */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void HandleInputAction(FName ActionName);

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /** Start the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void StartTutorial();

    /** Pause the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void PauseTutorial();

    /** Resume the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ResumeTutorial();

    /** Skip the tutorial */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void SkipTutorial();

    /** Get the current tutorial step */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    FTutorialStep GetCurrentStep() const;

    /** Get whether the tutorial is active */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    bool IsTutorialActive() const { return bTutorialActive; }

    /** Get whether the tutorial is paused */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    bool IsTutorialPaused() const { return bTutorialPaused; }

    /** Get the singleton instance */
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    static ATutorialManager* GetInstance(UObject* WorldContextObject);

    /** Events */
    UPROPERTY(BlueprintAssignable, Category = "Tutorial")
    FOnTutorialStepChanged OnTutorialStepChanged;

    UPROPERTY(BlueprintAssignable, Category = "Tutorial")
    FOnTutorialCompleted OnTutorialCompleted;
};
