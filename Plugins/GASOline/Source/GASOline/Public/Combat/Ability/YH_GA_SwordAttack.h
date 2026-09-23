//Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Maurice Sword Attack Ability
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "Combat/YH_CombatTypes.h"
#include "YH_GA_SwordAttack.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 *  * Sword attack ability for Player.
 * Activated by input tag YH.Combat.Ability.SwordAttack.
 * 
 * Flow:
 * 1. Checks CanAttack conditions via Gameplay Tags
 * 2. Waits for AnimNotify HitCheck event
 * 3. On HitCheck: sphere traces for enemies
 * 4. On hit: applies YH_GE_SwordDamage to enemy ASC
 * 5. Sends Hit.Confirmed event back to Blueprint for combo progression
 * 6. Ends ability — Blueprint handles combo counter and next input
 */
UCLASS()
class GASOLINE_API UYH_GA_SwordAttack : public UGASO_AbilityBase
{
	GENERATED_BODY()
	
public:
	UYH_GA_SwordAttack();

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
	void OnHitCheckReceived(FGameplayEventData Payload);
	
	// Performs hit trace and applied damage
	void PerformHitTrace(const FGameplayAbilityActorInfo* ActorInfo);

	UFUNCTION()
	void OnMontageEnd();
	// Gameplay Effect class to apply on sword hit
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Trace radius around Maurice's forward position
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float HitTraceRadius = 80.0f;

	// Trace distance forward from Maurice
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float HitTraceDistance = 150.0f;

	// Reference to the wait event task so we can end it cleanly
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitHitCheckTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Sword")
	float DamageAmount = 20.0f;
};
