//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Attack Damage Effect
// GASoline Plugin | Combat Module

#include "Combat/Effects/YH_GE_Damage.h"
#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_Damage::UYH_GE_Damage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - add to the target's IncomingDamage meta attribute. UYH_AttributeSet::
	//PostGameplayEffectExecute converts this into a real Health change and zeroes it back out.
	FGameplayModifierInfo ModifierInfo;

	ModifierInfo.Attribute = UYH_AttributeSet::GetIncomingDamageAttribute();

	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Set the magnitude to be determined by the caller (code or blueprint) when applying the effect,
	//using the "Data.Damage" tag. Positive value - IncomingDamage is always a non-negative amount.
	FSetByCallerFloat DamageMagnitude;

	DamageMagnitude.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));

	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageMagnitude);

	Modifiers.Add(ModifierInfo);
}


