// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Shared combat attribute set — base for every YH_* character (player and enemy alike).
// KHTry Game Module | Combat

#pragma once

#include "CoreMinimal.h"
#include "GASO_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "YH_AttributeSet.generated.h"

class AGASO_CharacterBase;

/**
 * Shared combat stats for every character in the game. Inherits Health/MaxHealth/Mana/MaxMana/
 * Stamina/MaxStamina from UGASO_AttributeSet (the generic GAS layer in the GASOline plugin);
 * everything game-specific lives here instead, so the plugin stays reusable.
 *
 * IncomingDamage/IncomingHealing are meta attributes: GameplayEffects never modify Health
 * directly anymore (see UYH_GE_Damage / UYH_GE_Heal) - they add a positive amount to one of
 * these instead. PostGameplayEffectExecute() reads it, applies the real Health change, and
 * zeroes the meta attribute back out so it never carries a stale value between hits. This is
 * what lets a future ExecutionCalculation (Strength/Magic vs. Defense/resistance) slot in later
 * without touching every ability that currently calls SetSetByCallerMagnitude directly.
 *
 * UYH_PlayerAttributeSet and UYH_EnemyAttributeSet both derive from this and stay deliberately
 * thin - this is where the logic they used to duplicate (HitReact/Death dispatch) now lives.
 */
UCLASS()
class KHTRY_API UYH_AttributeSet : public UGASO_AttributeSet
{
	GENERATED_BODY()

public:
	UYH_AttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called automatically by GAS after any GameplayEffect modifies an attribute. Converts
	// IncomingDamage/IncomingHealing into a real Health change, then dispatches HitReact/Death.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// --- Combat trio ---
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Attributes|Combat")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Magic, Category = "Attributes|Combat")
	FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, Magic)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Attributes|Combat")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, Defense)

	// --- Derived offense (scales outgoing GameplayEffects) ---
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicPower, Category = "Attributes|Combat")
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, MagicPower)

	// --- Mobility ---
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, Category = "Attributes|Combat")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, Speed)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeedMultiplier, Category = "Attributes|Combat")
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, MovementSpeedMultiplier)

	// --- Critical hits ---
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Critical, Category = "Attributes|Combat")
	FGameplayAttributeData Critical;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, Critical)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritMultiplier, Category = "Attributes|Combat")
	FGameplayAttributeData CritMultiplier;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, CritMultiplier)

	// --- Meta attributes: instant, local-only intermediate values. Not replicated - only the
	// resulting Health change needs to reach clients, which Health's own replication covers. ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, IncomingDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UYH_AttributeSet, IncomingHealing)

	// Guards against firing the Death event more than once (e.g. two lethal hits landing in the
	// same frame before the Death ability has a chance to react).
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Combat")
	bool bDeathStarted = false;

protected:
	UFUNCTION() void OnRep_Strength(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Magic(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MagicPower(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Speed(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Critical(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_CritMultiplier(const FGameplayAttributeData& OldValue);

private:
	// Shared HitReact/Death dispatch, used by both the damage and healing paths (only damage
	// triggers HitReact/Death, but both go through the same Health-changed bookkeeping).
	void HandleDamageTaken(float DamageAmount, AGASO_CharacterBase* OwnerCharacter);
};
