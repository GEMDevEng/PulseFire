#include "FlagBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Flag.h"
#include "../PulseFireCharacter.h"
#include "../Audio/SoundManager.h"
#include "../VFX/VFXManager.h"

// Sets default values
AFlagBase::AFlagBase()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set replication
    bReplicates = true;

    // Create components
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = CollisionComponent;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(RootComponent);
    BaseMesh->SetCollisionProfileName(TEXT("BlockAll"));

    BaseEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BaseEffect"));
    BaseEffect->SetupAttachment(RootComponent);

    // Set default values
    TeamIndex = 0;
    Flag = nullptr;
}

// Called when the game starts or when spawned
void AFlagBase::BeginPlay()
{
    Super::BeginPlay();
    
    // Bind overlap events
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlagBase::OnOverlapBegin);
}

// Called every frame
void AFlagBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFlagBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

    // Check if character is carrying a flag
    AFlag* CarriedFlag = nullptr;
    // In a real implementation, we would check if the character is carrying a flag
    // For now, we'll just use a placeholder
    
    if (CarriedFlag)
    {
        // Check if flag can be captured at this base
        if (CarriedFlag->GetTeamIndex() != TeamIndex)
        {
            // Capture flag
            CarriedFlag->Capture(this);

            // Broadcast capture event
            OnFlagCapturedAtBase.Broadcast(CarriedFlag, Character, this);

            // Play capture sound
            USoundManager::PlaySoundAtLocation(this, nullptr, GetActorLocation()); // Replace with actual sound

            // Play capture effect
            UVFXManager::SpawnEmitterAtLocation(this, nullptr, GetActorLocation()); // Replace with actual effect
        }
    }
}

void AFlagBase::SetTeamIndex(int32 NewTeamIndex)
{
    TeamIndex = NewTeamIndex;

    // Update base appearance based on team
    if (TeamIndex == 0)
    {
        // Red team
        BaseMesh->SetMaterial(0, nullptr); // Replace with actual material
    }
    else if (TeamIndex == 1)
    {
        // Blue team
        BaseMesh->SetMaterial(0, nullptr); // Replace with actual material
    }
}

void AFlagBase::SetFlag(AFlag* NewFlag)
{
    Flag = NewFlag;

    // Set flag's home base
    if (Flag)
    {
        Flag->SetHomeBase(this);
    }
}
