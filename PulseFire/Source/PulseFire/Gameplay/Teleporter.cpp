#include "Teleporter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATeleporter::ATeleporter()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetSphereRadius(100.0f);
    CollisionComp->SetCollisionProfileName("Trigger");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnOverlapBegin);
    RootComponent = CollisionComp;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    // Set default values
    CooldownTime = 1.0f;
    bIsOnCooldown = false;

    // Set mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshObj.Object);
        MeshComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.1f));
    }

    // Set material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/Materials/M_Teleporter"));
    if (MaterialObj.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialObj.Object);
    }
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATeleporter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && !bIsOnCooldown)
    {
        TeleportActor(OtherActor);
    }
}

void ATeleporter::TeleportActor(AActor* ActorToTeleport)
{
    if (!ActorToTeleport || !TargetTeleporter)
    {
        return;
    }

    // Set cooldown
    bIsOnCooldown = true;
    GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ATeleporter::ResetCooldown, CooldownTime, false);

    // Set target teleporter on cooldown
    TargetTeleporter->bIsOnCooldown = true;
    TargetTeleporter->GetWorldTimerManager().SetTimer(TargetTeleporter->TimerHandle_Cooldown, TargetTeleporter, &ATeleporter::ResetCooldown, CooldownTime, false);

    // Get target location and rotation
    FVector TargetLocation = TargetTeleporter->GetActorLocation();
    FRotator TargetRotation = TargetTeleporter->GetActorRotation();

    // Adjust height to prevent clipping
    TargetLocation.Z += 100.0f;

    // Teleport the actor
    ACharacter* Character = Cast<ACharacter>(ActorToTeleport);
    if (Character)
    {
        // For characters, preserve velocity
        FVector Velocity = Character->GetVelocity();
        
        // Teleport
        Character->SetActorLocationAndRotation(TargetLocation, TargetRotation);
        
        // Preserve velocity
        UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
        if (MovementComp)
        {
            MovementComp->Velocity = Velocity;
        }
    }
    else
    {
        // For other actors, just teleport
        ActorToTeleport->SetActorLocationAndRotation(TargetLocation, TargetRotation);
    }

    // Play teleport effects
    PlayTeleportEffects();
    TargetTeleporter->PlayTeleportEffects();
}

void ATeleporter::PlayTeleportEffects()
{
    // Play teleport sound
    if (TeleportSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation());
    }

    // Activate particle effect
    if (ParticleComp)
    {
        ParticleComp->Activate(true);
    }
}

void ATeleporter::ResetCooldown()
{
    bIsOnCooldown = false;
}
