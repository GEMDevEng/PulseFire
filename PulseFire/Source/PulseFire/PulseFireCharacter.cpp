// Copyright Epic Games, Inc. All Rights Reserved.

#include "PulseFireCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

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

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Set default values for health and ammo
	MaxHealth = 100.0f;
	Health = MaxHealth;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	bIsFiring = false;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void APulseFireCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void APulseFireCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APulseFireCharacter, Health);
	DOREPLIFETIME(APulseFireCharacter, CurrentAmmo);
	DOREPLIFETIME(APulseFireCharacter, bIsFiring);
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
	// Apply damage to health
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;
		
		// Check if character died
		if (Health <= 0.0f)
		{
			Health = 0.0f;
			OnDeath();
		}
	}
	
	return ActualDamage;
}

void APulseFireCharacter::OnDeath_Implementation()
{
	// Disable input
	DisableInput(Cast<APlayerController>(GetController()));
	
	// In a real implementation, we would play death animation and handle respawn
	// For now, just log the death
	UE_LOG(LogTemp, Warning, TEXT("%s has died"), *GetName());
}
