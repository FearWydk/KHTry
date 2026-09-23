// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Ability/YH_GA_EnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagsManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Combat/YH_PlayerAttributeSet.h"

UYH_GA_EnemyAttack::UYH_GA_EnemyAttack()
{
	// This ability is instanced per execution.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	// Ability tag - used to activate via TryActiveAbilitesByTag
	FGameplayTagContainer NewTags;
	NewTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.State.Attacking")));
	SetAssetTags(NewTags);
	// Block other attacks while this is active
	ActivationBlockedTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.State.Attacking")));
	
	for (const TCHAR* Variant : { TEXT("Light"), TEXT("Medium"), TEXT("Heavy") })
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("YH.Combat.Event.EnemyAttack.%s"), Variant)));
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UYH_GA_EnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FEnemyAttackData* AttackData = 
		TriggerEventData ? EnemyAttackDataMap.Find(TriggerEventData->EventTag) : nullptr;

	if (!AttackData|| !AttackData->AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("No attack data found for tag: %s"), *TriggerEventData->EventTag.ToString());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	FEnemyAttackData CurrentAttack = *AttackData;

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		CurrentAttack.AttackMontage);
}

void UYH_GA_EnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
