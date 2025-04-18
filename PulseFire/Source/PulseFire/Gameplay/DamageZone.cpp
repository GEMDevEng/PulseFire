#include "DamageZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
ADamageZone::ADamageZone()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    CollisionComp->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
    CollisionComp->SetCollisionProfileName("Trigger");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);
    RootComponent = CollisionComp;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    // Set default values
    DamagePerSecond = 10.0f;

    // Set mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObj.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshObj.Object);
        MeshComp->SetRelativeScale3D(FVector(4.0f, 4.0f, 2.0f));
    }

    // Set material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/Materials/M_DamageZone"));
    if (MaterialObj.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialObj.Object);
    }

    // Set collision properties
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetVisibility(true);
    MeshComp->SetHiddenInGame(false);
    MeshComp->SetOpacityMaskClipValue(0.5f);
    MeshComp->SetRenderCustomDepth(true);
}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
    Super::BeginPlay();
    
    // Start damage timer
    GetWorldTimerManager().SetTimer(TimerHandle_Damage, this, &ADamageZone::ApplyDamage, 1.0f, true);
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADamageZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        // Add actor to the list
        ActorsInZone.AddUnique(OtherActor);
    }
}

void ADamageZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        // Remove actor from the list
        ActorsInZone.Remove(OtherActor);
    }
}

void ADamageZone::ApplyDamage()
{
    // Only apply damage on the server
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Apply damage to all actors in the zone
    for (AActor* Actor : ActorsInZone)
    {
        if (Actor && !Actor->IsPendingKill())
        {
            // Apply damage
            FDamageEvent DamageEvent(DamageType ? DamageType : UDamageType::StaticClass());
            Actor->TakeDamage(DamagePerSecond, DamageEvent, GetInstigatorController(), this);
        }
    }
}
