#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLayout.generated.h"

class UBoxComponent;
class UArrowComponent;
class UBillboardComponent;
class ABasePickup;
class ACapturePoint;
class APulseFirePlayerStart;
class AJumpPad;
class ATeleporter;
class ADamageZone;

/**
 * Helper class for map layout design.
 * This class helps visualize and place map elements in the editor.
 */
UCLASS()
class PULSEFIRE_API AMapLayout : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AMapLayout();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Root component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* RootBox;

    /** Billboard component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBillboardComponent* BillboardComp;

    /** Map name */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    FString MapName;

    /** Map description */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    FString MapDescription;

    /** Map author */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    FString MapAuthor;

    /** Map version */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    FString MapVersion;

    /** Map size (small, medium, large) */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    FString MapSize;

    /** Recommended player count */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    int32 RecommendedPlayerCount;

    /** Maximum player count */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    int32 MaxPlayerCount;

    /** Number of teams */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    int32 NumTeams;

    /** Spawn points in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<APulseFirePlayerStart*> SpawnPoints;

    /** Capture points in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<ACapturePoint*> CapturePoints;

    /** Pickups in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<ABasePickup*> Pickups;

    /** Jump pads in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<AJumpPad*> JumpPads;

    /** Teleporters in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<ATeleporter*> Teleporters;

    /** Damage zones in the map */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Map")
    TArray<ADamageZone*> DamageZones;

#if WITH_EDITOR
    /** Called when a property is changed in the editor */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    /** Update visualization */
    void UpdateVisualization();

    /** Find all map elements */
    void FindMapElements();
#endif

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
