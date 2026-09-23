// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASO_AbilityBase.h"
#include "YH_GA_EnemyAttack.generated.h"

class UAbilityTask_PlayMontageAndWait;

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

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<FGameplayTag, FEnemyAttackData> EnemyAttackDataMap;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FEnemyAttackData CurrentAttackData;

	
	
};
