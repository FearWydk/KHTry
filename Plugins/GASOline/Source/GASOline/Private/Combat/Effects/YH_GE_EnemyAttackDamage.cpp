//Josh Brooks Copyright 2026
// Elemental Action RPG — The Severance
// Enemy Attack Damage Effect
// GASoline Plugin | Combat Module


#include "Combat/Effects/YH_GE_EnemyAttackDamage.h"
#include "Combat/YH_PlayerAttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_EnemyAttackDamage::UYH_GE_EnemyAttackDamage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - subtract 15.0 from the target's Health
	FGameplayModifierInfo ModifierInfo;

	//Target the Health attribute on the target
	ModifierInfo.Attribute = UYH_PlayerAttributeSet::GetHealthAttribute();

	//Subtract from Health
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Scalable float magnitude - negative value = damage
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-15.0f));
	Modifiers.Add(ModifierInfo);
}
