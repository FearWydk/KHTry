// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "YH_GA_EnemyAttack.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

USTRUCT(BlueprintType)
struct FEnemyAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr <UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TraceRange = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TraceRadius = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FGameplayTagContainer AttackTags;
};

/**
 *
 */

UCLASS()
class GASOLINE_API UYH_GA_EnemyAttack : public UGASO_AbilityBase
{
	GENERATED_BODY()

public:

	UYH_GA_EnemyAttack();

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

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	// Waits for the AnimNotify HitCheck event from the attack montage (mirrors YH_GA_SwordAttack).
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitHitCheckTask;

	UFUNCTION()
	void OnMontageEnd();

	// Listens for AnimNotify HitCheck event from Blueprint
	UFUNCTION()
	void OnHitCheckReceived(FGameplayEventData Payload);

	// Performs the hit trace and applies damage using CurrentAttackData
	void PerformHitTrace(const FGameplayAbilityActorInfo* ActorInfo);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<FGameplayTag, FEnemyAttackData> EnemyAttackDataMap;

	// Gameplay Effect class to apply on a landed hit. Magnitude comes from CurrentAttackData.Damage via SetByCaller "Data.Damage".
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Cached on the ability instance (InstancedPerExecution) from EnemyAttackDataMap in ActivateAbility;
	// read by PerformHitTrace on the HitCheck notify.
	FEnemyAttackData CurrentAttackData;
};
