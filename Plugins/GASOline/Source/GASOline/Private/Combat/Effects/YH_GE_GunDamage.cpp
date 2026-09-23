//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Gun Attack Damage Effect
// GASoline Plugin | Combat Module

#include "Combat/Effects/YH_GE_GunDamage.h"
#include "Combat/YH_PlayerAttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_GunDamage::UYH_GE_GunDamage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - subtract 8.0 from the target's Health
	FGameplayModifierInfo ModifierInfo;

	//Target the Health attribute on the target
	ModifierInfo.Attribute = UYH_PlayerAttributeSet::GetHealthAttribute();

	//Subtract from Health
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Scalable float magnitude - negative value = damage
	FGameplayEffectModifierMagnitude Magnitude;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-8.0f));

	Modifiers.Add(ModifierInfo);




}
