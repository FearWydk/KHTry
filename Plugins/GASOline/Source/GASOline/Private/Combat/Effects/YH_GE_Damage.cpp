//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Attack Damage Effect
// GASoline Plugin | Combat Module

#include "Combat/Effects/YH_GE_Damage.h"
#include "Combat/YH_PlayerAttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_Damage::UYH_GE_Damage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - subtract 10.0 from the target's Health
	FGameplayModifierInfo ModifierInfo;

	//Target the Health attribute on the target
	ModifierInfo.Attribute = UYH_PlayerAttributeSet::GetHealthAttribute();


	//Subtract from Health
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Set the magnitude to be determined by the caller (code or blueprint) when applying the effect, using the "Data.Damage" tag to identify which value to use
	FSetByCallerFloat DamageMagnitude;

	DamageMagnitude.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));

	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageMagnitude);

	Modifiers.Add(ModifierInfo);
}


