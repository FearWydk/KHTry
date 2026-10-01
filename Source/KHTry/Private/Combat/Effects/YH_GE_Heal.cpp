//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Instant Heal Effect
// GASoline Plugin | Combat Module

#include "Combat/Effects/YH_GE_Heal.h"
#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_Heal::UYH_GE_Heal()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - add to the target's IncomingHealing meta attribute. UYH_AttributeSet::
	//PostGameplayEffectExecute converts this into a real Health change and zeroes it back out.
	FGameplayModifierInfo ModifierInfo;

	ModifierInfo.Attribute = UYH_AttributeSet::GetIncomingHealingAttribute();

	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Set the magnitude to be determined by the caller (code or blueprint) when applying the effect, using the "Data.Heal" tag to identify which value to use
	FSetByCallerFloat HealMagnitude;

	HealMagnitude.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Heal"));

	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealMagnitude);

	Modifiers.Add(ModifierInfo);
}
