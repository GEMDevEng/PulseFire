#include "AssaultRifle.h"
#include "../PulseFireCharacter.h"
#include "GameFramework/PlayerController.h"

AAssaultRifle::AAssaultRifle()
{
    // Set assault rifle specific values
    WeaponType = EWeaponType::Rifle;
    FireMode = EFireMode::Auto;
    WeaponName = "Assault Rifle";
    BaseDamage = 25.0f;
    HeadshotMultiplier = 2.5f;
    RateOfFire = 600.0f; // 600 RPM
    MaxAmmo = 30;
    MaxReserveAmmo = 120;
    HipFireSpread = 2.5f;
    AimSpread = 0.3f;
    ReloadTime = 2.2f;
    EquipTime = 1.0f;
    Range = 10000.0f;
    VerticalRecoil = 1.2f;
    HorizontalRecoil = 0.6f;
    RecoilRecovery = 4.0f;
    AimDownSightSpeed = 0.6f;
    AimFOV = 65.0f;

    // Initialize recoil pattern
    RecoilPattern.Add(FVector2D(0.0f, 0.8f));   // Shot 1: Mostly up
    RecoilPattern.Add(FVector2D(0.2f, 1.0f));   // Shot 2: Up and slightly right
    RecoilPattern.Add(FVector2D(0.4f, 1.2f));   // Shot 3: Up and more right
    RecoilPattern.Add(FVector2D(0.6f, 1.0f));   // Shot 4: Up and right
    RecoilPattern.Add(FVector2D(0.4f, 0.8f));   // Shot 5: Less up, still right
    RecoilPattern.Add(FVector2D(0.2f, 1.0f));   // Shot 6: More up, less right
    RecoilPattern.Add(FVector2D(0.0f, 1.2f));   // Shot 7: Straight up, more
    RecoilPattern.Add(FVector2D(-0.2f, 1.0f));  // Shot 8: Up and slightly left
    RecoilPattern.Add(FVector2D(-0.4f, 0.8f));  // Shot 9: Up and more left
    RecoilPattern.Add(FVector2D(-0.6f, 1.0f));  // Shot 10: Up and left
    RecoilPattern.Add(FVector2D(-0.4f, 1.2f));  // Shot 11: More up, less left
    RecoilPattern.Add(FVector2D(-0.2f, 1.0f));  // Shot 12: Up and slightly left
    RecoilPattern.Add(FVector2D(0.0f, 0.8f));   // Shot 13: Straight up
    RecoilPattern.Add(FVector2D(0.3f, 1.0f));   // Shot 14: Up and right
    RecoilPattern.Add(FVector2D(0.5f, 1.2f));   // Shot 15: Up and more right

    CurrentRecoilIndex = 0;
}

void AAssaultRifle::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize ammo
    CurrentAmmo = MaxAmmo;
    CurrentReserveAmmo = MaxReserveAmmo;
}

void AAssaultRifle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAssaultRifle::ApplyRecoil()
{
    // Get recoil values from pattern
    FVector2D RecoilAmount = RecoilPattern[CurrentRecoilIndex];
    
    // Scale recoil based on aiming
    if (bIsAiming)
    {
        RecoilAmount *= 0.5f;
    }

    // Apply recoil to player's camera
    APulseFireCharacter* Character = GetPulseFireCharacter();
    if (Character)
    {
        APlayerController* PC = Cast<APlayerController>(Character->GetController());
        if (PC)
        {
            // Apply vertical recoil
            PC->AddPitchInput(-RecoilAmount.Y * 0.05f);

            // Apply horizontal recoil
            PC->AddYawInput(RecoilAmount.X * 0.05f);
        }
    }

    // Increment recoil index
    CurrentRecoilIndex = (CurrentRecoilIndex + 1) % RecoilPattern.Num();
}
