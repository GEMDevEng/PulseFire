#include "CapturePoint.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "../PulseFireCharacter.h"
#include "../Multiplayer/PulseFirePlayerState.h"

// Sets default values
ACapturePoint::ACapturePoint()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
    CollisionComp->SetCapsuleHalfHeight(150.0f);
    CollisionComp->SetCapsuleRadius(250.0f);
    CollisionComp->SetCollisionProfileName("Trigger");
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACapturePoint::OnOverlapBegin);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACapturePoint::OnOverlapEnd);
    RootComponent = CollisionComp;

    BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
    BaseMeshComp->SetupAttachment(RootComponent);
    BaseMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -150.0f));

    FlagMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMeshComp"));
    FlagMeshComp->SetupAttachment(BaseMeshComp);
    FlagMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    // Set default values
    CaptureTime = 10.0f;
    CaptureProgress = 0.0f;
    CapturingTeam = 0;
    OwningTeam = 0;

    // Set base mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (BaseMeshObj.Succeeded())
    {
        BaseMeshComp->SetStaticMesh(BaseMeshObj.Object);
        BaseMeshComp->SetRelativeScale3D(FVector(5.0f, 5.0f, 0.5f));
    }

    // Set flag mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlagMeshObj(TEXT("/Engine/BasicShapes/Plane"));
    if (FlagMeshObj.Succeeded())
    {
        FlagMeshComp->SetStaticMesh(FlagMeshObj.Object);
        FlagMeshComp->SetRelativeScale3D(FVector(2.0f, 1.0f, 2.0f));
        FlagMeshComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    }

    // Set replication
    bReplicates = true;
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACapturePoint::BeginPlay()
{
    Super::BeginPlay();
    
    // Start capture update timer
    GetWorldTimerManager().SetTimer(TimerHandle_CaptureUpdate, this, &ACapturePoint::UpdateCapture, 0.1f, true);

    // Update appearance
    UpdateAppearance();
}

// Called every frame
void ACapturePoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACapturePoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(ACapturePoint, CaptureProgress);
    DOREPLIFETIME(ACapturePoint, CapturingTeam);
    DOREPLIFETIME(ACapturePoint, OwningTeam);
}

void ACapturePoint::OnRep_CaptureProgress()
{
    // Broadcast state change
    OnCapturePointStateChanged.Broadcast(CapturingTeam, CaptureProgress);
}

void ACapturePoint::OnRep_CapturingTeam()
{
    // Update appearance
    UpdateAppearance();

    // Broadcast state change
    OnCapturePointStateChanged.Broadcast(CapturingTeam, CaptureProgress);
}

void ACapturePoint::OnRep_OwningTeam()
{
    // Update appearance
    UpdateAppearance();

    // Play capture effects
    PlayCaptureEffects();

    // Broadcast capture
    OnCapturePointCaptured.Broadcast(OwningTeam);
}

void ACapturePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        // Check if the actor is a character
        APulseFireCharacter* Character = Cast<APulseFireCharacter>(OtherActor);
        if (Character)
        {
            // Add character to the list
            CharactersInZone.AddUnique(Character);
        }
    }
}

void ACapturePoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        // Check if the actor is a character
        APulseFireCharacter* Character = Cast<APulseFireCharacter>(OtherActor);
        if (Character)
        {
            // Remove character from the list
            CharactersInZone.Remove(Character);
        }
    }
}

void ACapturePoint::UpdateCapture()
{
    // Only update on the server
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Get team counts
    TMap<int32, int32> TeamCounts;
    GetTeamCounts(TeamCounts);

    // Find the team with the most players
    int32 DominantTeam = 0;
    int32 MaxCount = 0;
    bool bContested = false;

    for (const auto& Pair : TeamCounts)
    {
        if (Pair.Value > MaxCount)
        {
            MaxCount = Pair.Value;
            DominantTeam = Pair.Key;
            bContested = false;
        }
        else if (Pair.Value == MaxCount && MaxCount > 0)
        {
            bContested = true;
        }
    }

    // If contested, no team is capturing
    if (bContested)
    {
        DominantTeam = 0;
    }

    // Update capturing team
    if (CapturingTeam != DominantTeam)
    {
        CapturingTeam = DominantTeam;
        OnRep_CapturingTeam();
    }

    // Update capture progress
    if (CapturingTeam != 0)
    {
        // If capturing team is different from owning team, progress towards capture
        if (CapturingTeam != OwningTeam)
        {
            // Progress towards capture
            float ProgressStep = 1.0f / (CaptureTime * 10.0f); // 10 updates per second
            CaptureProgress += ProgressStep;

            // Check if capture is complete
            if (CaptureProgress >= 1.0f)
            {
                CaptureProgress = 0.0f;
                OwningTeam = CapturingTeam;
                OnRep_OwningTeam();
            }
            else
            {
                OnRep_CaptureProgress();
            }
        }
        else
        {
            // Reset progress if the capturing team is the same as the owning team
            if (CaptureProgress > 0.0f)
            {
                CaptureProgress = 0.0f;
                OnRep_CaptureProgress();
            }
        }
    }
    else
    {
        // No team is capturing, gradually reset progress
        if (CaptureProgress > 0.0f)
        {
            float ProgressStep = 1.0f / (CaptureTime * 20.0f); // Slower reset
            CaptureProgress = FMath::Max(0.0f, CaptureProgress - ProgressStep);
            OnRep_CaptureProgress();
        }
    }
}

void ACapturePoint::PlayCaptureEffects()
{
    // Play capture sound
    if (CaptureSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CaptureSound, GetActorLocation());
    }

    // Activate particle effect
    if (ParticleComp)
    {
        ParticleComp->Activate(true);
    }
}

void ACapturePoint::UpdateAppearance()
{
    // Get team color
    FLinearColor TeamColor = FLinearColor::White;
    
    if (OwningTeam != 0)
    {
        // Create a temporary player state to get the team color
        APulseFirePlayerState TempPlayerState;
        TempPlayerState.SetTeamIndex(OwningTeam);
        TeamColor = TempPlayerState.GetTeamColor();
    }

    // Create a dynamic material instance for the flag
    UMaterialInstanceDynamic* FlagMaterial = FlagMeshComp->CreateAndSetMaterialInstanceDynamic(0);
    if (FlagMaterial)
    {
        FlagMaterial->SetVectorParameterValue(TEXT("Color"), TeamColor);
    }

    // Create a dynamic material instance for the base
    UMaterialInstanceDynamic* BaseMaterial = BaseMeshComp->CreateAndSetMaterialInstanceDynamic(0);
    if (BaseMaterial)
    {
        BaseMaterial->SetVectorParameterValue(TEXT("Color"), TeamColor);
    }
}

void ACapturePoint::GetTeamCounts(TMap<int32, int32>& TeamCounts)
{
    // Count characters by team
    for (APulseFireCharacter* Character : CharactersInZone)
    {
        if (Character)
        {
            // Get player state
            APulseFirePlayerState* PlayerState = Character->GetPlayerState<APulseFirePlayerState>();
            if (PlayerState)
            {
                // Get team index
                int32 TeamIndex = PlayerState->GetTeamIndex();
                if (TeamIndex > 0)
                {
                    // Increment team count
                    int32& Count = TeamCounts.FindOrAdd(TeamIndex);
                    Count++;
                }
            }
        }
    }
}
