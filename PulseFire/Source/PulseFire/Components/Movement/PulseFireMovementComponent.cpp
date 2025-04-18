#include "PulseFireMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

UPulseFireMovementComponent::UPulseFireMovementComponent()
{
	// Set default values
	BaseWalkSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	AimSpeedMultiplier = 0.6f;
	bIsSprinting = false;
	bIsAiming = false;

	// Set initial movement speeds
	MaxWalkSpeed = BaseWalkSpeed;
	MaxWalkSpeedCrouched = BaseWalkSpeed * 0.5f;

	// Set other movement properties
	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanJump = true;
	bCanWalkOffLedgesWhenCrouching = true;
	JumpZVelocity = 420.0f;
	AirControl = 0.2f;
	BrakingDecelerationWalking = 2000.0f;
}

void UPulseFireMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize movement speeds
	UpdateMovementSpeeds();
}

void UPulseFireMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Additional tick logic can be added here
}

void UPulseFireMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(UPulseFireMovementComponent, bIsSprinting);
	DOREPLIFETIME(UPulseFireMovementComponent, bIsAiming);
}

void UPulseFireMovementComponent::SetSprinting(bool bNewSprinting)
{
	// Only allow sprinting when not aiming and not crouching
	if (bNewSprinting && (bIsAiming || IsCrouching()))
	{
		bNewSprinting = false;
	}

	// Update sprinting state
	if (bIsSprinting != bNewSprinting)
	{
		// Update local value
		bIsSprinting = bNewSprinting;

		// Update movement speeds
		UpdateMovementSpeeds();

		// Replicate to server if we're a client
		if (GetOwnerRole() < ROLE_Authority)
		{
			ServerSetSprinting(bNewSprinting);
		}
	}
}

bool UPulseFireMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}

void UPulseFireMovementComponent::SetAiming(bool bNewAiming)
{
	// Update aiming state
	if (bIsAiming != bNewAiming)
	{
		// Update local value
		bIsAiming = bNewAiming;

		// Can't sprint while aiming
		if (bIsAiming && bIsSprinting)
		{
			SetSprinting(false);
		}

		// Update movement speeds
		UpdateMovementSpeeds();

		// Replicate to server if we're a client
		if (GetOwnerRole() < ROLE_Authority)
		{
			ServerSetAiming(bNewAiming);
		}
	}
}

bool UPulseFireMovementComponent::IsAiming() const
{
	return bIsAiming;
}

void UPulseFireMovementComponent::UpdateMovementSpeeds()
{
	// Calculate new max walk speed
	float NewMaxWalkSpeed = BaseWalkSpeed;

	// Apply sprint multiplier if sprinting
	if (bIsSprinting)
	{
		NewMaxWalkSpeed *= SprintSpeedMultiplier;
	}
	// Apply aim multiplier if aiming
	else if (bIsAiming)
	{
		NewMaxWalkSpeed *= AimSpeedMultiplier;
	}

	// Update movement speeds
	MaxWalkSpeed = NewMaxWalkSpeed;
	MaxWalkSpeedCrouched = BaseWalkSpeed * 0.5f; // Crouching is always half of base speed
}

bool UPulseFireMovementComponent::ServerSetSprinting_Validate(bool bNewSprinting)
{
	return true;
}

void UPulseFireMovementComponent::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	SetSprinting(bNewSprinting);
}

bool UPulseFireMovementComponent::ServerSetAiming_Validate(bool bNewAiming)
{
	return true;
}

void UPulseFireMovementComponent::ServerSetAiming_Implementation(bool bNewAiming)
{
	SetAiming(bNewAiming);
}
