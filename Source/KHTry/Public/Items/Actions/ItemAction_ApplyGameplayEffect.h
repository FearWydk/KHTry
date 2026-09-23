// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Actions/ItemAction.h"
#include "Templates/SubclassOf.h"
#include "ItemAction_ApplyGameplayEffect.generated.h"

class UGameplayEffect;

/**
 * Item Action that applies a GameplayEffect to the item user's AbilitySystemComponent on use.
 * Data-driven wrapper around the same MakeOutgoingSpec / SetByCaller flow the combat abilities
 * use to apply damage (see UYH_GA_SwordAttack::PerformHitTrace) - only here it is applied to
 * the item owner instead of a hit target, which makes it equally suited to a healing potion
 * (UYH_GE_HealOverTime, tag "Data.Heal") as to a self-applied damage/DoT or buff effect.
 *
 * ItemOwner must have an AbilitySystemComponent (via IAbilitySystemInterface) for this to succeed.
 */
UCLASS(DisplayName = "Apply Gameplay Effect")
class KHTRY_API UItemAction_ApplyGameplayEffect : public UItemAction
{
	GENERATED_BODY()

public:

	// GameplayEffect to apply to the item owner, e.g. UYH_GE_HealOverTime to heal on use.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> EffectClass;

	// Level the effect is applied at.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float EffectLevel = 1.0f;

	// If valid, the effect's SetByCaller magnitude registered under this tag is set to Magnitude
	// before the effect is applied (e.g. "Data.Heal" for UYH_GE_HealOverTime). Leave unset if the
	// effect class does not use SetByCaller.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FGameplayTag SetByCallerTag;

	// Magnitude sent via SetByCaller when SetByCallerTag is valid. For an Additive Health modifier,
	// use a positive value to heal (matches UYH_GE_HealOverTime's "Data.Heal" convention).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Magnitude = 0.0f;

	virtual bool Execute_Implementation(AActor* ItemOwner, UItemInstance* ItemInst) override;
};
