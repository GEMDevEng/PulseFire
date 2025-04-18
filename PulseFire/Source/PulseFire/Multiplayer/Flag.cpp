#include "Flag.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "../PulseFireCharacter.h"
#include "FlagBase.h"
#include "CaptureTheFlagGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Audio/SoundManager.h"
#include "../VFX/VFXManager.h"

// Sets default values
AFlag::AFlag()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set replication
    bReplicates = true;
    SetReplicateMovement(true);

    // Create components
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetSphereRadius(100.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = CollisionComponent;

    FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
    FlagMesh->SetupAttachment(RootComponent);
    FlagMesh->SetCollisionProfileName(TEXT("NoCollision"));
    FlagMesh->SetGenerateOverlapEvents(false);

    FlagEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlagEffect"));
    FlagEffect->SetupAttachment(RootComponent);

    // Set default values
    TeamIndex = 0;
    HomeBase = nullptr;
    Carrier = nullptr;
    bIsAtHome = true;
    bIsBeingCarried = false;
    bIsOnGround = false;
    AttachSocketName = FName("FlagSocket");
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind overlap events
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnOverlapBegin);
}

// Called every frame
void AFlag::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(AFlag, Carrier);
    DOREPLIFETIME(AFlag, bIsAtHome);
    DOREPLIFETIME(AFlag, bIsBeingCarried);
    DOREPLIFETIME(AFlag, bIsOnGround);
}

void AFlag::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Check if overlapping with character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(OtherActor);
    if (!Character)
    {
        return;
    }

    // Check if flag is already being carried
    if (bIsBeingCarried)
    {
        return;
    }

    // Check if character is on the same team
    if (Character->GetPlayerState() && Character->GetPlayerState()->GetTeamIndex() == TeamIndex)
    {
        // Same team, return flag if it's not at home
        if (!bIsAtHome)
        {
            ReturnToBase();
        }
    }
    else
    {
        // Enemy team, pickup flag
        Pickup(Character);
    }
}

void AFlag::SetTeamIndex(int32 NewTeamIndex)
{
    TeamIndex = NewTeamIndex;

    // Update flag appearance based on team
    if (TeamIndex == 0)
    {
        // Red team
        FlagMesh->SetMaterial(0, nullptr); // Replace with actual material
    }
    else if (TeamIndex == 1)
    {
        // Blue team
        FlagMesh->SetMaterial(0, nullptr); // Replace with actual material
    }
}

void AFlag::SetHomeBase(AFlagBase* NewHomeBase)
{
    HomeBase = NewHomeBase;
}

void AFlag::SetCarrier(APulseFireCharacter* NewCarrier)
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerPickup(NewCarrier);
        return;
    }

    // Set carrier
    Carrier = NewCarrier;
    bIsBeingCarried = true;
    bIsAtHome = false;
    bIsOnGround = false;

    // Attach flag to carrier
    if (Carrier)
    {
        AttachToComponent(Carrier->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
    }

    // Cancel return timer
    CancelReturnTimer();

    // Broadcast pickup event
    OnFlagPickedUp.Broadcast(this, Carrier);

    // Notify game mode
    ACaptureTheFlagGameMode* GameMode = Cast<ACaptureTheFlagGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnFlagPickedUp(this, Carrier);
    }

    // Play pickup sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound
}

void AFlag::ClearCarrier()
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerDrop();
        return;
    }

    // Store carrier for event
    APulseFireCharacter* OldCarrier = Carrier;

    // Clear carrier
    Carrier = nullptr;
    bIsBeingCarried = false;
    bIsOnGround = true;

    // Detach flag from carrier
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // Enable collision
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Broadcast drop event
    OnFlagDropped.Broadcast(this, OldCarrier);

    // Notify game mode
    ACaptureTheFlagGameMode* GameMode = Cast<ACaptureTheFlagGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnFlagDropped(this, OldCarrier);
    }

    // Play drop sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound
}

void AFlag::Pickup(APulseFireCharacter* Character)
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerPickup(Character);
        return;
    }

    // Set carrier
    SetCarrier(Character);
}

void AFlag::Drop()
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerDrop();
        return;
    }

    // Clear carrier
    ClearCarrier();
}

void AFlag::Capture(AFlagBase* FlagBase)
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerCapture(FlagBase);
        return;
    }

    // Check if flag can be captured
    if (!bIsBeingCarried || !Carrier || !FlagBase)
    {
        return;
    }

    // Check if capturing at the correct base
    if (FlagBase->GetTeamIndex() == TeamIndex)
    {
        return;
    }

    // Store carrier for event
    APulseFireCharacter* OldCarrier = Carrier;

    // Broadcast capture event
    OnFlagCaptured.Broadcast(this, OldCarrier, FlagBase);

    // Notify game mode
    ACaptureTheFlagGameMode* GameMode = Cast<ACaptureTheFlagGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnFlagCaptured(this, OldCarrier, FlagBase);
    }

    // Return flag to base
    ReturnToBase();

    // Play capture sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound

    // Play capture effect
    UVFXManager::SpawnEmitterAtLocation(this, nullptr, GetActorLocation()); // Replace with actual effect
}

void AFlag::ReturnToBase()
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        ServerReturn();
        return;
    }

    // Return flag to base
    ReturnToBaseInternal();
}

void AFlag::ReturnToBaseInternal()
{
    // Clear carrier
    Carrier = nullptr;
    bIsBeingCarried = false;
    bIsOnGround = false;
    bIsAtHome = true;

    // Detach flag from carrier
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // Move flag to home base
    if (HomeBase)
    {
        SetActorLocation(HomeBase->GetActorLocation() + FVector(0, 0, 50.0f));
        SetActorRotation(HomeBase->GetActorRotation());
    }

    // Enable collision
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // Cancel return timer
    CancelReturnTimer();

    // Broadcast return event
    OnFlagReturned.Broadcast(this);

    // Notify game mode
    ACaptureTheFlagGameMode* GameMode = Cast<ACaptureTheFlagGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->OnFlagReturned(this);
    }

    // Play return sound
    USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound

    // Play return effect
    UVFXManager::SpawnEmitterAtLocation(this, nullptr, GetActorLocation()); // Replace with actual effect
}

void AFlag::StartReturnTimer(float ReturnTime)
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Start timer to return flag
    GetWorldTimerManager().SetTimer(TimerHandle_FlagReturn, this, &AFlag::ReturnToBaseInternal, ReturnTime, false);
}

void AFlag::CancelReturnTimer()
{
    // Only process on server
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Cancel return timer
    GetWorldTimerManager().ClearTimer(TimerHandle_FlagReturn);
}

bool AFlag::ServerPickup_Validate(APulseFireCharacter* Character)
{
    return true;
}

void AFlag::ServerPickup_Implementation(APulseFireCharacter* Character)
{
    Pickup(Character);
}

bool AFlag::ServerDrop_Validate()
{
    return true;
}

void AFlag::ServerDrop_Implementation()
{
    Drop();
}

bool AFlag::ServerCapture_Validate(AFlagBase* FlagBase)
{
    return true;
}

void AFlag::ServerCapture_Implementation(AFlagBase* FlagBase)
{
    Capture(FlagBase);
}

bool AFlag::ServerReturn_Validate()
{
    return true;
}

void AFlag::ServerReturn_Implementation()
{
    ReturnToBase();
}
