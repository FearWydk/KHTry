//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Gun Attack Damage Effect
// GASoline Plugin | Combat Module

#include "Combat/Effects/YH_GE_GunDamage.h"
#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_GunDamage::UYH_GE_GunDamage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - add to the target's IncomingDamage meta attribute. UYH_AttributeSet::
	//PostGameplayEffectExecute converts this into a real Health change and zeroes it back out.
	FGameplayModifierInfo ModifierInfo;

	ModifierInfo.Attribute = UYH_AttributeSet::GetIncomingDamageAttribute();

	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Scalable float magnitude - positive value, IncomingDamage is always non-negative
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(8.0f));

	Modifiers.Add(ModifierInfo);
}
