// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Effects/YH_GE_AOE_Damage.h"

#include "GASO_AttributeSet.h"
#include "GameplayEffectTypes.h"

UYH_GE_AOE_Damage::UYH_GE_AOE_Damage()
{
	//Instant effect - applies once and does not linger
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	//Periodic effect - applies every 1 second
	//Period = 1.0f;
	Period = FScalableFloat(1.0f);
	//Wait the first second
	bExecutePeriodicEffectOnApplication = false;
	//Build the modifier - subtract 10.0 from the target's Health
	FGameplayModifierInfo ModifierInfo;

	//Target the Health attribute on the target
	ModifierInfo.Attribute = UGASO_AttributeSet::GetHealthAttribute();


	//Subtract from Health
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	//Set the magnitude to be determined by the caller (code or blueprint) when applying the effect, using the "Data.Damage" tag to identify which value to use
	FSetByCallerFloat DamageMagnitude;

	DamageMagnitude.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));

	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageMagnitude);

	Modifiers.Add(ModifierInfo);
}
