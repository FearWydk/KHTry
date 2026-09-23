// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "Combat/YH_CombatTypes.h"
#include "Combat/YH_CombatStatics.h"
#include "YH_GA_HitReact.generated.h"

class UAbilityTask_PlayMontageAndWait;

/**
 * Hit React Abiltiy for GASO Character.
 * Activated by Event Tag. YH.Combat.Event.HitReact(To be updated to direction reaction later in development).
 */
UCLASS()
class GASOLINE_API UYH_GA_HitReact : public UGASO_AbilityBase
{
	GENERATED_BODY()

public:

	UYH_GA_HitReact();

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
		bool bReplciateEndAbility,
		bool bWasCancelled
	) override;

protected:
	//Reference to Hit Reaction Montage

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

public:

	UPROPERTY(EditDefaultsOnly) UAnimMontage* HitReactMontage;


	UFUNCTION()
	void OnMontageEnd();
};
