// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Effects/YH_GE_AOE_Damage.h"

#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_AOE_Damage::UYH_GE_AOE_Damage()
{
	//Infinite periodic effect - keeps applying until removed
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	//Periodic effect - applies every 1 second
	Period = FScalableFloat(1.0f);
	//Wait the first second
	bExecutePeriodicEffectOnApplication = false;

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
