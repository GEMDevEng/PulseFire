#include "MapLayout.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Pickups/BasePickup.h"
#include "CapturePoint.h"
#include "../Multiplayer/PulseFirePlayerStart.h"
#include "JumpPad.h"
#include "Teleporter.h"
#include "DamageZone.h"

// Sets default values
AMapLayout::AMapLayout()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
    RootBox->SetBoxExtent(FVector(2000.0f, 2000.0f, 500.0f));
    RootBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootBox->SetHiddenInGame(true);
    RootComponent = RootBox;

    BillboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComp"));
    BillboardComp->SetupAttachment(RootComponent);

    // Set default values
    MapName = "New Map";
    MapDescription = "A new map for PulseFire";
    MapAuthor = "Unknown";
    MapVersion = "1.0";
    MapSize = "Medium";
    RecommendedPlayerCount = 8;
    MaxPlayerCount = 16;
    NumTeams = 2;

    // Set editor properties
    bIsEditorOnly = true;
}

// Called when the game starts or when spawned
void AMapLayout::BeginPlay()
{
    Super::BeginPlay();
    
    // Destroy in game
    Destroy();
}

// Called every frame
void AMapLayout::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR
    // Draw debug info in editor
    if (GIsEditor && !GIsPlayInEditorWorld)
    {
        // Draw map bounds
        DrawDebugBox(GetWorld(), GetActorLocation(), RootBox->GetScaledBoxExtent(), FColor::Green, false, -1.0f, 0, 5.0f);

        // Draw spawn points
        for (APulseFirePlayerStart* SpawnPoint : SpawnPoints)
        {
            if (SpawnPoint)
            {
                FColor SpawnColor = FColor::White;
                if (SpawnPoint->TeamIndex > 0)
                {
                    // Use team color
                    switch (SpawnPoint->TeamIndex)
                    {
                    case 1:
                        SpawnColor = FColor::Blue;
                        break;
                    case 2:
                        SpawnColor = FColor::Red;
                        break;
                    case 3:
                        SpawnColor = FColor::Green;
                        break;
                    case 4:
                        SpawnColor = FColor::Yellow;
                        break;
                    }
                }

                DrawDebugSphere(GetWorld(), SpawnPoint->GetActorLocation(), 50.0f, 12, SpawnColor, false, -1.0f, 0, 2.0f);
                DrawDebugDirectionalArrow(GetWorld(), SpawnPoint->GetActorLocation(), SpawnPoint->GetActorLocation() + SpawnPoint->GetActorForwardVector() * 100.0f, 50.0f, SpawnColor, false, -1.0f, 0, 2.0f);
            }
        }

        // Draw capture points
        for (ACapturePoint* CapturePoint : CapturePoints)
        {
            if (CapturePoint)
            {
                DrawDebugCylinder(GetWorld(), CapturePoint->GetActorLocation() - FVector(0, 0, 100), CapturePoint->GetActorLocation() + FVector(0, 0, 100), 250.0f, 32, FColor::Purple, false, -1.0f, 0, 2.0f);
            }
        }

        // Draw pickups
        for (ABasePickup* Pickup : Pickups)
        {
            if (Pickup)
            {
                DrawDebugSphere(GetWorld(), Pickup->GetActorLocation(), 50.0f, 12, FColor::Cyan, false, -1.0f, 0, 2.0f);
            }
        }

        // Draw jump pads
        for (AJumpPad* JumpPad : JumpPads)
        {
            if (JumpPad)
            {
                DrawDebugBox(GetWorld(), JumpPad->GetActorLocation(), FVector(50, 50, 10), FColor::Yellow, false, -1.0f, 0, 2.0f);
                
                // Draw launch direction
                FVector LaunchDir = JumpPad->GetActorForwardVector().RotateAngleAxis(45.0f, JumpPad->GetActorRightVector());
                DrawDebugDirectionalArrow(GetWorld(), JumpPad->GetActorLocation(), JumpPad->GetActorLocation() + LaunchDir * 200.0f, 50.0f, FColor::Yellow, false, -1.0f, 0, 2.0f);
            }
        }

        // Draw teleporters
        for (ATeleporter* Teleporter : Teleporters)
        {
            if (Teleporter)
            {
                DrawDebugCylinder(GetWorld(), Teleporter->GetActorLocation() - FVector(0, 0, 50), Teleporter->GetActorLocation() + FVector(0, 0, 50), 100.0f, 32, FColor::Magenta, false, -1.0f, 0, 2.0f);
            }
        }

        // Draw damage zones
        for (ADamageZone* DamageZone : DamageZones)
        {
            if (DamageZone)
            {
                DrawDebugBox(GetWorld(), DamageZone->GetActorLocation(), FVector(200, 200, 100), FColor::Red, false, -1.0f, 0, 2.0f);
            }
        }
    }
#endif
}

#if WITH_EDITOR
void AMapLayout::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Update visualization when properties change
    UpdateVisualization();
}

void AMapLayout::UpdateVisualization()
{
    // Update billboard text
    if (BillboardComp)
    {
        BillboardComp->SetText(FText::FromString(MapName));
    }

    // Find map elements
    FindMapElements();
}

void AMapLayout::FindMapElements()
{
    // Clear arrays
    SpawnPoints.Empty();
    CapturePoints.Empty();
    Pickups.Empty();
    JumpPads.Empty();
    Teleporters.Empty();
    DamageZones.Empty();

    // Find all map elements in the level
    TArray<AActor*> FoundActors;

    // Find spawn points
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APulseFirePlayerStart::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        APulseFirePlayerStart* SpawnPoint = Cast<APulseFirePlayerStart>(Actor);
        if (SpawnPoint)
        {
            SpawnPoints.Add(SpawnPoint);
        }
    }

    // Find capture points
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturePoint::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        ACapturePoint* CapturePoint = Cast<ACapturePoint>(Actor);
        if (CapturePoint)
        {
            CapturePoints.Add(CapturePoint);
        }
    }

    // Find pickups
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePickup::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        ABasePickup* Pickup = Cast<ABasePickup>(Actor);
        if (Pickup)
        {
            Pickups.Add(Pickup);
        }
    }

    // Find jump pads
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJumpPad::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        AJumpPad* JumpPad = Cast<AJumpPad>(Actor);
        if (JumpPad)
        {
            JumpPads.Add(JumpPad);
        }
    }

    // Find teleporters
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeleporter::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        ATeleporter* Teleporter = Cast<ATeleporter>(Actor);
        if (Teleporter)
        {
            Teleporters.Add(Teleporter);
        }
    }

    // Find damage zones
    FoundActors.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADamageZone::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        ADamageZone* DamageZone = Cast<ADamageZone>(Actor);
        if (DamageZone)
        {
            DamageZones.Add(DamageZone);
        }
    }
}
#endif
