#include "HealthPickup.h"
#include "../PulseFireCharacter.h"
#include "../Components/HealthComponent.h"

AHealthPickup::AHealthPickup()
{
    // Set default values
    HealthAmount = 50.0f;

    // Set mesh and material
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Sphere"));
    if (MeshObj.Succeeded())
    {
        MeshComp->SetStaticMesh(MeshObj.Object);
        MeshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    }

    // Set material
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/Materials/M_HealthPickup"));
    if (MaterialObj.Succeeded())
    {
        MeshComp->SetMaterial(0, MaterialObj.Object);
    }
}

void AHealthPickup::OnPickedUp_Implementation(APulseFireCharacter* Character)
{
    if (Character)
    {
        // Get health component
        UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
        if (HealthComp)
        {
            // Only pickup if health is not full
            if (HealthComp->GetHealth() < HealthComp->GetMaxHealth())
            {
                // Heal the character
                HealthComp->Heal(HealthAmount);

                // Call parent implementation
                Super::OnPickedUp_Implementation(Character);
            }
        }
    }
}
