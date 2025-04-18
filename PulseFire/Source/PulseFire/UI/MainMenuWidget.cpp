#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../PulseFireGameInstance.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsButtonClicked);
    }

    if (CreditsButton)
    {
        CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
    }

    if (HostGameButton)
    {
        HostGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostGameButtonClicked);
    }

    if (JoinGameButton)
    {
        JoinGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinGameButtonClicked);
    }

    if (FindGamesButton)
    {
        FindGamesButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnFindGamesButtonClicked);
    }

    if (BackFromMultiplayerButton)
    {
        BackFromMultiplayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackFromMultiplayerButtonClicked);
    }

    if (BackFromOptionsButton)
    {
        BackFromOptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackFromOptionsButtonClicked);
    }

    if (BackFromCreditsButton)
    {
        BackFromCreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackFromCreditsButtonClicked);
    }

    // Set initial menu panel
    if (MenuSwitcher && MainMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(MainMenuPanel);
    }
}

void UMainMenuWidget::OnPlayButtonClicked()
{
    if (MenuSwitcher && MultiplayerMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(MultiplayerMenuPanel);
    }
}

void UMainMenuWidget::OnOptionsButtonClicked()
{
    if (MenuSwitcher && OptionsMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(OptionsMenuPanel);
    }
}

void UMainMenuWidget::OnCreditsButtonClicked()
{
    if (MenuSwitcher && CreditsMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(CreditsMenuPanel);
    }
}

void UMainMenuWidget::OnQuitButtonClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
}

void UMainMenuWidget::OnHostGameButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->HostSession("PulseFire Game", false);
    }
}

void UMainMenuWidget::OnJoinGameButtonClicked()
{
    // This would open a dialog to enter IP address
    // For now, we'll just join a local game
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->JoinSessionByIP("127.0.0.1");
    }
}

void UMainMenuWidget::OnFindGamesButtonClicked()
{
    UPulseFireGameInstance* GameInstance = Cast<UPulseFireGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->FindSessions(false);
    }
}

void UMainMenuWidget::OnBackFromMultiplayerButtonClicked()
{
    if (MenuSwitcher && MainMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(MainMenuPanel);
    }
}

void UMainMenuWidget::OnBackFromOptionsButtonClicked()
{
    if (MenuSwitcher && MainMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(MainMenuPanel);
    }
}

void UMainMenuWidget::OnBackFromCreditsButtonClicked()
{
    if (MenuSwitcher && MainMenuPanel)
    {
        MenuSwitcher->SetActiveWidget(MainMenuPanel);
    }
}
