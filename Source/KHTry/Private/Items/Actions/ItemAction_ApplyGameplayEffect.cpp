// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Actions/ItemAction_ApplyGameplayEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

bool UItemAction_ApplyGameplayEffect::Execute_Implementation(AActor* ItemOwner, UItemInstance* ItemInst)
{
	if (!ItemOwner || !EffectClass)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ItemOwner);
	if (!ASC)
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ItemOwner);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, EffectLevel, EffectContext);
	if (!SpecHandle.IsValid())
	{
		return false;
	}

	if (SetByCallerTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(SetByCallerTag, Magnitude);
	}

	const FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	return ActiveHandle.WasSuccessfullyApplied();
}
