// Josh Brooks Copyright 2026

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "Combat/YH_CombatTypes.h"
#include "Combat/YH_CombatStatics.h"
#include "YH_GA_Finisher.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
/**
 * Sword Finisher ability for Player.
 * Activated by input tag YH.Combat.Ability.SwordFinisher, which is fired by the combo manager when the player reaches the max combo count.
 * 
 * Flow:
 * 1. Activated by combo manager when player reaches max combo count.
 * 2. Plays finisher montage, which has a notify to apply the finisher damage effect at the right moment.
 * 3. Applies finisher damage effect to enemies in range, which scales based on the combo result (e.g. more hits = more damage).
 * 4. Ends ability — combo manager resets the combo count and allows the player to start a new chain.
 */
UCLASS()
class GASOLINE_API UYH_GA_Finisher : public UGASO_AbilityBase
{
	GENERATED_BODY()

public:
	UYH_GA_Finisher();
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

	// Listens for AnimNotify HitCheck event from Blueprint
	UFUNCTION()
	void OnFinisherHitCheckReceived(FGameplayEventData Payload);

	
	// Performs hit trace and applies finisher damage, FUTURE: will be scaling based on combo count
	void PerformFinisherHitTrace(const FGameplayAbilityActorInfo* ActorInfo);

	UFUNCTION()
	void OnMontageEnd();

	// Gameplay Effect class to apply on finisher hit, which scales based on the combo result (e.g. more hits = more damage).
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Trace radius around the player for the finisher hit check.
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float FinisherHitTraceRadius = 130.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float HitTraceDistance = 200.0f;

	//Reference to the currently playing montage, so we can stop it when the ability ends.
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitHitCheckTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float DamageAmount = 20.0f;
};
