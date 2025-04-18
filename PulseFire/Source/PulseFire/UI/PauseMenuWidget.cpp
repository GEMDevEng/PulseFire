#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../PulseFireGameInstance.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeButtonClicked);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionsButtonClicked);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitButtonClicked);
    }
}

void UPauseMenuWidget::OnResumeButtonClicked()
{
    // Resume the game
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        // Hide the pause menu
        HideWithAnimation();

        // Unpause the game
        PlayerController->SetPause(false);

        // Show the cursor
        PlayerController->bShowMouseCursor = false;

        // Set input mode to game
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
}

void UPauseMenuWidget::OnOptionsButtonClicked()
{
    // Show options menu
    // This would be implemented in the game-specific code
}

void UPauseMenuWidget::OnMainMenuButtonClicked()
{
    // Return to main menu
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        // Destroy the current session
        GameInstance->DestroySession();
    }
}

void UPauseMenuWidget::OnQuitButtonClicked()
{
    // Quit the game
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
}
