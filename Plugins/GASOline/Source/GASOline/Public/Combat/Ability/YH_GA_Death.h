// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "Combat/YH_CombatTypes.h"
#include "Combat/YH_CombatStatics.h"
#include "YH_GA_Death.generated.h"


class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitDelay;

/**
 * Death ability for GASO Character.
 * Activated by input tag YH.Combat.Ability.Death, which is fired once actors health is <= 0.
 *
 * Flow:
 * 1. Activated once AttributeSet detects Health less than or equal to 0.
 * 2. Plays death montage.
 * 3. Plays fading/actor destruction.
 * 4. Ends ability — actor should be fully destoryed by end.
 */
UCLASS()
class GASOLINE_API UYH_GA_Death : public UGASO_AbilityBase
{
	GENERATED_BODY()

public:
	UYH_GA_Death();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
protected:
	//Reference to the currently playing montage, so we can stop it when the ability ends.

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> WaitDelayTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	

public:

	UPROPERTY(EditDefaultsOnly) UAnimMontage* DeathMontage;

	UFUNCTION()
	void OnMontageEnd();
};
