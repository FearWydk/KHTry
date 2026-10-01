// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Effects/YH_GE_HealOverTime.h"
#include "Combat/YH_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_HealOverTime::UYH_GE_HealOverTime()
{
	//Infinite periodic effect - keeps applying until removed
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	//Periodic effect - applies every 1 second
	Period = FScalableFloat(1.0f);
	//Apply immediately on application, then every period after
	bExecutePeriodicEffectOnApplication = true;

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
