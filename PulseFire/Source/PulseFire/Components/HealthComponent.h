#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

/**
 * Component to handle health and damage for actors.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULSEFIRE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Event fired when health changes */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	/** Get current health */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	/** Get maximum health */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	/** Get health as a percentage (0-1) */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;

	/** Handle damage received by owner */
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Set the current health directly */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth);

	/** Heal the character by the specified amount */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

protected:
	/** Maximum health of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	/** Current health of the character */
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	float Health;

	/** Whether the character is dead */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	/** Replication function for health */
	UFUNCTION()
	void OnRep_Health(float OldHealth);

	/** Get the owning character */
	UFUNCTION(BlueprintCallable, Category = "Health")
	class ACharacter* GetOwningCharacter() const;
};
