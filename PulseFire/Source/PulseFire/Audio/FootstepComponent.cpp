#include "FootstepComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SoundManager.h"

// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame
    PrimaryComponentTick.bCanEverTick = true;

    // Set default values
    VolumeMultiplier = 1.0f;
    PitchVariation = 0.1f;
    MinSpeedToPlaySound = 10.0f;
    TraceDistance = 100.0f;
    LastFootstepTime = 0.0f;
}

// Called when the game starts
void UFootstepComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check for footstep sounds
    CheckForFootstep();
}

void UFootstepComponent::CheckForFootstep()
{
    // Get owner character
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    // Get character movement component
    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (!MovementComp)
    {
        return;
    }

    // Check if character is moving
    float Speed = MovementComp->Velocity.Size2D();
    if (Speed < MinSpeedToPlaySound || !MovementComp->IsMovingOnGround())
    {
        return;
    }

    // Calculate time between footsteps based on speed
    float TimeBetweenSteps = 0.5f;
    if (MovementComp->IsCrouching())
    {
        TimeBetweenSteps = 0.6f;
    }
    else if (Speed > MovementComp->MaxWalkSpeed * 0.8f)
    {
        TimeBetweenSteps = 0.3f;
    }

    // Check if it's time for a footstep
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFootstepTime >= TimeBetweenSteps)
    {
        PlayFootstepSound();
        LastFootstepTime = CurrentTime;
    }
}

void UFootstepComponent::PlayFootstepSound()
{
    // Get owner character
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    // Get character movement component
    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (!MovementComp)
    {
        return;
    }

    // Get surface type
    TEnumAsByte<EPhysicalSurface> SurfaceType = GetSurfaceType();

    // Get movement state
    bool bIsRunning = MovementComp->Velocity.Size2D() > MovementComp->MaxWalkSpeed * 0.8f;
    bool bIsCrouching = MovementComp->IsCrouching();

    // Get appropriate sound
    USoundBase* FootstepSound = GetFootstepSound(SurfaceType, bIsRunning, bIsCrouching);
    if (!FootstepSound)
    {
        return;
    }

    // Calculate pitch variation
    float PitchMultiplier = 1.0f + FMath::RandRange(-PitchVariation, PitchVariation);

    // Play sound
    USoundManager::PlaySoundAtLocation(GetWorld(), FootstepSound, Character->GetActorLocation(), VolumeMultiplier, PitchMultiplier);
}

USoundBase* UFootstepComponent::GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType, bool bIsRunning, bool bIsCrouching)
{
    // Get the appropriate sound set
    FFootstepSounds* SoundSet = &WalkSounds;
    if (bIsCrouching)
    {
        SoundSet = &CrouchSounds;
    }
    else if (bIsRunning)
    {
        SoundSet = &RunSounds;
    }

    // Get sound based on surface type
    USoundBase* Sound = nullptr;
    switch (SurfaceType)
    {
    case SURFACE_CONCRETE:
        Sound = SoundSet->ConcreteSound;
        break;
    case SURFACE_DIRT:
        Sound = SoundSet->DirtSound;
        break;
    case SURFACE_METAL:
        Sound = SoundSet->MetalSound;
        break;
    case SURFACE_WOOD:
        Sound = SoundSet->WoodSound;
        break;
    case SURFACE_WATER:
        Sound = SoundSet->WaterSound;
        break;
    case SURFACE_GRASS:
        Sound = SoundSet->GrassSound;
        break;
    case SURFACE_GLASS:
        Sound = SoundSet->GlassSound;
        break;
    default:
        Sound = SoundSet->DefaultSound;
        break;
    }

    // Fall back to default sound if needed
    if (!Sound)
    {
        Sound = SoundSet->DefaultSound;
    }

    return Sound;
}

TEnumAsByte<EPhysicalSurface> UFootstepComponent::GetSurfaceType()
{
    // Get owner character
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return EPhysicalSurface::SurfaceType_Default;
    }

    // Trace from feet downward
    FVector Start = Character->GetActorLocation();
    FVector End = Start - FVector(0, 0, TraceDistance);

    // Set trace parameters
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);
    QueryParams.bReturnPhysicalMaterial = true;

    // Perform trace
    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
    {
        return UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
    }

    return EPhysicalSurface::SurfaceType_Default;
}
