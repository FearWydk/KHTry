//Josh Brooks Copyright 2026
// Elemental Action RPG — The Severance
// Enemy Attack Damage Effect
// GASoline Plugin | Combat Module


#include "Combat/Effects/YH_GE_EnemyAttackDamage.h"
#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_EnemyAttackDamage::UYH_GE_EnemyAttackDamage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Instant;

	//Build the modifier - add to the target's IncomingDamage meta attribute. UYH_AttributeSet::
	//PostGameplayEffectExecute converts this into a real Health change and zeroes it back out.
	FGameplayModifierInfo ModifierInfo;

	ModifierInfo.Attribute = UYH_AttributeSet::GetIncomingDamageAttribute();

	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Scalable float magnitude - positive value, IncomingDamage is always non-negative
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(15.0f));
	Modifiers.Add(ModifierInfo);
}
