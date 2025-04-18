#include "JumpPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJumpPad::AJumpPad()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    CollisionComp->SetBoxExtent(FVector(50.0f, 50.0f, 10.0f));
    CollisionComp->SetCollisionProfileName("Trigger");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnOverlapBegin);
    RootComponent = CollisionComp;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));

    ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
    ArrowComp->SetupAttachment(RootComponent);
    ArrowComp->SetRelativeRotation(FRotator(45.0f, 0.0f, 0.0f));
    ArrowComp->SetArrowColor(FColor::Yellow);
    ArrowComp->SetArrowSize(2.0f);
    ArrowComp->bHiddenInGame = false;

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    // Set default values
    LaunchStrength = 1500.0f;
    LaunchAngle = 45.0f;

    // Set mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Plane"));
    if (MeshObj.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshObj.Object);
        MeshComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.1f));
    }

    // Set material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/Materials/M_JumpPad"));
    if (MaterialObj.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialObj.Object);
    }
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
    Super::BeginPlay();
    
    // Update arrow rotation based on launch angle
    ArrowComp->SetRelativeRotation(FRotator(LaunchAngle, 0.0f, 0.0f));
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AJumpPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        LaunchActor(OtherActor);
    }
}

void AJumpPad::LaunchActor(AActor* ActorToLaunch)
{
    if (!ActorToLaunch)
    {
        return;
    }

    // Calculate launch velocity
    FVector LaunchDirection = ArrowComp->GetForwardVector();
    FVector LaunchVelocity = LaunchDirection * LaunchStrength;

    // Apply launch velocity
    ACharacter* Character = Cast<ACharacter>(ActorToLaunch);
    if (Character)
    {
        // For characters, use LaunchCharacter
        Character->LaunchCharacter(LaunchVelocity, true, true);
    }
    else
    {
        // For other actors, apply impulse to physics
        UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorToLaunch->GetRootComponent());
        if (PrimComp && PrimComp->IsSimulatingPhysics())
        {
            PrimComp->AddImpulse(LaunchVelocity, NAME_None, true);
        }
    }

    // Play launch effects
    PlayLaunchEffects();
}

void AJumpPad::PlayLaunchEffects()
{
    // Play launch sound
    if (LaunchSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
    }

    // Activate particle effect
    if (ParticleComp)
    {
        ParticleComp->Activate(true);
    }
}
