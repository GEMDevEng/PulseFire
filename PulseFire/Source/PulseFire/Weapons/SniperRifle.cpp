#include "SniperRifle.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "../PulseFireCharacter.h"
#include "../Audio/SoundManager.h"

// Sets default values
ASniperRifle::ASniperRifle()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    WeaponType = EWeaponType::SniperRifle;
    FireMode = EFireMode::Single;
    BaseDamage = 100.0f;
    HeadshotMultiplier = 3.0f;
    RateOfFire = 60.0f; // 1 shot per second
    MaxAmmo = 5;
    MaxReserveAmmo = 25;
    CurrentAmmo = MaxAmmo;
    CurrentReserveAmmo = MaxReserveAmmo;
    HipFireSpread = 5.0f;
    AimSpread = 0.1f;
    ReloadTime = 3.0f;
    EquipTime = 1.5f;
    Range = 20000.0f;
    VerticalRecoil = 3.0f;
    HorizontalRecoil = 1.0f;
    RecoilRecovery = 3.0f;
    AimDownSightSpeed = 0.3f;
    AimFOV = 70.0f;
    WeaponName = "Sniper Rifle";

    // Sniper-specific values
    ScopeFOV = 30.0f;
    bIsScoped = false;
    ScopeInTime = 0.3f;
    ScopeOutTime = 0.2f;
    BreathHoldTime = 5.0f;
    CurrentBreathHoldTime = BreathHoldTime;
    BreathHoldSwayReduction = 0.9f;
    bIsHoldingBreath = false;
}

// Called when the game starts or when spawned
void ASniperRifle::BeginPlay()
{
    Super::BeginPlay();

    // Create scope widget if we're the local player
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character && Character->IsLocallyControlled() && ScopeWidgetClass)
    {
        APlayerController* PC = Cast<APlayerController>(Character->GetController());
        if (PC)
        {
            ScopeWidget = CreateWidget<UUserWidget>(PC, ScopeWidgetClass);
        }
    }

    // Store default FOV
    if (Character)
    {
        UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
        if (Camera)
        {
            DefaultFOV = Camera->FieldOfView;
        }
    }
}

// Called every frame
void ASniperRifle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update breath hold time
    if (bIsHoldingBreath)
    {
        CurrentBreathHoldTime = FMath::Max(0.0f, CurrentBreathHoldTime - DeltaTime);
        if (CurrentBreathHoldTime <= 0.0f)
        {
            StopHoldBreath();
        }
    }
}

void ASniperRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(ASniperRifle, bIsScoped);
    DOREPLIFETIME(ASniperRifle, bIsHoldingBreath);
}

void ASniperRifle::SetAiming(bool bNewAiming)
{
    // Call parent implementation
    Super::SetAiming(bNewAiming);

    // Handle scoping
    if (bIsAiming)
    {
        // Start scope-in transition
        GetWorldTimerManager().SetTimer(TimerHandle_ScopeTransition, this, &ASniperRifle::ShowScopeOverlay, ScopeInTime, false);
    }
    else
    {
        // Cancel scope-in if it's in progress
        GetWorldTimerManager().ClearTimer(TimerHandle_ScopeTransition);

        // Hide scope overlay immediately
        HideScopeOverlay();

        // Stop holding breath
        if (bIsHoldingBreath)
        {
            StopHoldBreath();
        }
    }
}

void ASniperRifle::ShowScopeOverlay()
{
    // Set scoped state
    bIsScoped = true;

    // Show scope widget
    if (ScopeWidget && !ScopeWidget->IsInViewport())
    {
        ScopeWidget->AddToViewport();
    }

    // Set FOV
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
        if (Camera)
        {
            Camera->SetFieldOfView(ScopeFOV);
        }

        // Hide weapon mesh when scoped
        if (MeshComp)
        {
            MeshComp->SetVisibility(false);
        }
    }

    // Play scope sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound
}

void ASniperRifle::HideScopeOverlay()
{
    // Set scoped state
    bIsScoped = false;

    // Hide scope widget
    if (ScopeWidget && ScopeWidget->IsInViewport())
    {
        ScopeWidget->RemoveFromViewport();
    }

    // Reset FOV
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
        if (Camera)
        {
            Camera->SetFieldOfView(bIsAiming ? AimFOV : DefaultFOV);
        }

        // Show weapon mesh when not scoped
        if (MeshComp)
        {
            MeshComp->SetVisibility(true);
        }
    }

    // Play scope sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound
}

void ASniperRifle::StartHoldBreath()
{
    // Only allow holding breath when scoped
    if (!bIsScoped || CurrentBreathHoldTime <= 0.0f)
    {
        return;
    }

    // Set holding breath state
    bIsHoldingBreath = true;

    // Play breath hold sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound

    // Replicate to server
    if (GetLocalRole() < ROLE_Authority)
    {
        ServerSetHoldingBreath(true);
    }
}

void ASniperRifle::StopHoldBreath()
{
    // Only do something if we're holding breath
    if (!bIsHoldingBreath)
    {
        return;
    }

    // Set holding breath state
    bIsHoldingBreath = false;

    // Play breath release sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound

    // Start breath recovery
    GetWorldTimerManager().SetTimer(TimerHandle_BreathHold, this, &ASniperRifle::ResetBreath, BreathHoldTime * 2.0f, false);

    // Replicate to server
    if (GetLocalRole() < ROLE_Authority)
    {
        ServerSetHoldingBreath(false);
    }
}

void ASniperRifle::ResetBreath()
{
    // Reset breath hold time
    CurrentBreathHoldTime = BreathHoldTime;
}

bool ASniperRifle::ServerSetHoldingBreath_Validate(bool bNewHoldingBreath)
{
    return true;
}

void ASniperRifle::ServerSetHoldingBreath_Implementation(bool bNewHoldingBreath)
{
    bIsHoldingBreath = bNewHoldingBreath;
}

float ASniperRifle::GetCurrentSwayReduction() const
{
    return bIsHoldingBreath ? BreathHoldSwayReduction : 0.0f;
}
