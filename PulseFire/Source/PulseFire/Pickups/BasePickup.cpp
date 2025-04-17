#include "BasePickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "../PulseFireCharacter.h"

// Sets default values
ABasePickup::ABasePickup()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetSphereRadius(50.0f);
    CollisionComp->SetCollisionProfileName("Pickup");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnOverlapBegin);
    RootComponent = CollisionComp;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

    RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
    RotatingMovement->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    // Set default values
    RespawnTime = 30.0f;
    bIsActive = true;

    // Set replication
    bReplicates = true;
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(ABasePickup, bIsActive);
}

void ABasePickup::OnRep_IsActive()
{
    // Update visibility based on active state
    SetActorHiddenInGame(!bIsActive);
    SetActorEnableCollision(bIsActive);

    // Update particle effects
    if (ParticleComp)
    {
        if (bIsActive)
        {
            ParticleComp->Activate();
        }
        else
        {
            ParticleComp->Deactivate();
        }
    }
}

void ABasePickup::OnPickedUp_Implementation(APulseFireCharacter* Character)
{
    // Play pickup effects
    PlayPickupEffects();

    // Deactivate the pickup
    if (GetLocalRole() == ROLE_Authority)
    {
        SetActive(false);

        // Start respawn timer if needed
        if (RespawnTime > 0.0f)
        {
            GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ABasePickup::Respawn, RespawnTime, false);
        }
    }
}

void ABasePickup::OnRespawn_Implementation()
{
    // Activate the pickup
    if (GetLocalRole() == ROLE_Authority)
    {
        SetActive(true);
    }
}

void ABasePickup::Respawn()
{
    OnRespawn();
}

void ABasePickup::PlayPickupEffects()
{
    // Play pickup sound
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }
}

void ABasePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Only process if we're active and on the server
    if (!bIsActive || GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Check if the overlapping actor is a character
    APulseFireCharacter* Character = Cast<APulseFireCharacter>(OtherActor);
    if (Character)
    {
        OnPickedUp(Character);
    }
}

void ABasePickup::SetActive(bool bNewActive)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        bIsActive = bNewActive;
        OnRep_IsActive();
    }
}
