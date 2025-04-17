#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set default values
	MaxHealth = 100.0f;
	Health = MaxHealth;
	bIsDead = false;

	// Set this component to be initialized when the game starts, and to be ticked every frame
	PrimaryComponentTick.bCanEverTick = false;

	// Set replication
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hook up damage events on server
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	}

	// Initialize health
	Health = MaxHealth;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	// Update health clamped
	const float OldHealth = Health;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	const float ActualDamage = OldHealth - Health;
	if (ActualDamage > 0.0f)
	{
		// Fire health changed event
		OnHealthChanged.Broadcast(this, Health, -ActualDamage, DamageType, InstigatedBy, DamageCauser);

		// Check for death
		if (Health <= 0.0f && !bIsDead)
		{
			bIsDead = true;
			
			// Notify character of death
			ACharacter* OwningCharacter = GetOwningCharacter();
			if (OwningCharacter)
			{
				// You would typically call a death function on the character here
				// For example: OwningCharacter->OnDeath();
			}
		}
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		const float OldHealth = Health;
		Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

		const float HealthDelta = Health - OldHealth;
		if (HealthDelta != 0.0f)
		{
			OnHealthChanged.Broadcast(this, Health, HealthDelta, nullptr, nullptr, nullptr);
		}
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount > 0.0f && Health < MaxHealth)
	{
		SetHealth(Health + HealAmount);
	}
}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	const float HealthDelta = Health - OldHealth;
	if (HealthDelta != 0.0f)
	{
		OnHealthChanged.Broadcast(this, Health, HealthDelta, nullptr, nullptr, nullptr);
	}

	// Check for death
	if (Health <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		
		// Notify character of death
		ACharacter* OwningCharacter = GetOwningCharacter();
		if (OwningCharacter)
		{
			// You would typically call a death function on the character here
			// For example: OwningCharacter->OnDeath();
		}
	}
}

ACharacter* UHealthComponent::GetOwningCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}
