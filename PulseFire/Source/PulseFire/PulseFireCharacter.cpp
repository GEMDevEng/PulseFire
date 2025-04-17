// Copyright Epic Games, Inc. All Rights Reserved.

#include "PulseFireCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/Movement/PulseFireMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/Weapon.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

APulseFireCharacter::APulseFireCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// Only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(FirstPersonCameraComponent);
	FP_Gun->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// Create a muzzle location
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Create health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Set default values for ammo and state
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	bIsFiring = false;
	bIsAiming = false;
	bIsSprinting = false;

	// Use custom movement component
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 420.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void APulseFireCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	// Spawn the default weapon if we're the server
	if (GetLocalRole() == ROLE_Authority)
	{
		// Spawn a default weapon
		// Note: In a real implementation, we would spawn the weapon based on a weapon class property
		// For now, we'll just use the weapon we've already created
		// FActorSpawnParameters SpawnParams;
		// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// SpawnParams.Owner = this;
		// CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		// if (CurrentWeapon)
		// {
		// 	CurrentWeapon->AttachToComponent(FP_Gun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("GripPoint")));
		// }
	}
}

void APulseFireCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APulseFireCharacter, CurrentAmmo);
	DOREPLIFETIME(APulseFireCharacter, bIsFiring);
	DOREPLIFETIME(APulseFireCharacter, bIsAiming);
	DOREPLIFETIME(APulseFireCharacter, bIsSprinting);
	DOREPLIFETIME(APulseFireCharacter, CurrentWeapon);
}

void APulseFireCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APulseFireCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APulseFireCharacter::LookUp(float Value)
{
	// Calculate new pitch
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APulseFireCharacter::Turn(float Value)
{
	// Calculate new yaw
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APulseFireCharacter::StartSprint()
{
	// Only allow sprinting when not aiming
	if (!bIsAiming)
	{
		bIsSprinting = true;
		GetPulseFireMovementComponent()->SetSprinting(true);
	}
}

void APulseFireCharacter::StopSprint()
{
	bIsSprinting = false;
	GetPulseFireMovementComponent()->SetSprinting(false);
}

void APulseFireCharacter::StartAiming()
{
	// Can't aim while sprinting
	if (bIsSprinting)
	{
		StopSprint();
	}

	bIsAiming = true;
	GetPulseFireMovementComponent()->SetAiming(true);

	// Adjust FOV for aiming
	FirstPersonCameraComponent->SetFieldOfView(70.0f); // Zoomed in FOV
}

void APulseFireCharacter::StopAiming()
{
	bIsAiming = false;
	GetPulseFireMovementComponent()->SetAiming(false);

	// Reset FOV
	FirstPersonCameraComponent->SetFieldOfView(90.0f); // Default FOV
}

void APulseFireCharacter::StartCrouch()
{
	Crouch();
}

void APulseFireCharacter::StopCrouch()
{
	UnCrouch();
}

void APulseFireCharacter::Jump()
{
	Super::Jump();
}

void APulseFireCharacter::StopJumping()
{
	Super::StopJumping();
}

void APulseFireCharacter::StartFire()
{
	if (CurrentAmmo > 0 && !bIsFiring)
	{
		bIsFiring = true;
		ServerFire();
	}
}

void APulseFireCharacter::StopFire()
{
	bIsFiring = false;
}

void APulseFireCharacter::Reload()
{
	if (CurrentAmmo < MaxAmmo)
	{
		// In a real implementation, we would play reload animation and add a delay
		CurrentAmmo = MaxAmmo;
	}
}

bool APulseFireCharacter::ServerFire_Validate()
{
	// Simple validation to prevent cheating
	return (CurrentAmmo > 0);
}

void APulseFireCharacter::ServerFire_Implementation()
{
	// Decrement ammo
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;

		// Perform line trace to determine hit
		FHitResult HitResult = PerformLineTrace();

		// If we hit something, apply damage
		if (HitResult.GetActor())
		{
			// Apply damage to the hit actor
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(20.0f, DamageEvent, GetController(), this);
		}

		// Play fire effects (sound, animation, etc.)
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		if (FireAnimation)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = FP_Gun->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

FHitResult APulseFireCharacter::PerformLineTrace() const
{
	// Get the camera transform
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Calculate line trace end point
	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f); // 10 meters

	// Set up trace parameters
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	// Perform line trace
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	// Draw debug line (only in development builds)
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
#endif

	return HitResult;
}

float APulseFireCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Let the health component handle damage
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Health component will handle death via its delegate
	return ActualDamage;
}

void APulseFireCharacter::OnDeath_Implementation()
{
	// Disable input
	DisableInput(Cast<APlayerController>(GetController()));

	// Stop any movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Drop weapon if we have one
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon = nullptr;
	}

	// In a real implementation, we would play death animation and handle respawn
	// For now, just log the death
	UE_LOG(LogTemp, Warning, TEXT("%s has died"), *GetName());

	// Respawn after delay (in a real game, this would be handled by the game mode)
	FTimerHandle TimerHandle_Respawn;
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &APulseFireCharacter::Respawn, 5.0f, false);
}

void APulseFireCharacter::Respawn()
{
	// In a real implementation, this would be handled by the game mode
	// For testing purposes, we'll just restore health and re-enable input
	if (HealthComponent)
	{
		HealthComponent->SetHealth(HealthComponent->GetMaxHealth());
	}

	// Re-enable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Re-enable movement
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Re-enable input
	EnableInput(Cast<APlayerController>(GetController()));
}

void APulseFireCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APulseFireCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APulseFireCharacter::StopFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APulseFireCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APulseFireCharacter::MoveRight);

	// Bind looking events
	PlayerInputComponent->BindAxis("Turn", this, &APulseFireCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APulseFireCharacter::LookUp);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APulseFireCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APulseFireCharacter::StopCrouch);

	// Bind sprint events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APulseFireCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APulseFireCharacter::StopSprint);

	// Bind aim events
	PlayerInputComponent->BindAction("AimDownSights", IE_Pressed, this, &APulseFireCharacter::StartAiming);
	PlayerInputComponent->BindAction("AimDownSights", IE_Released, this, &APulseFireCharacter::StopAiming);

	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APulseFireCharacter::Reload);
}
