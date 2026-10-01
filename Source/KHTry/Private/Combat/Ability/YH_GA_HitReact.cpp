// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Ability/YH_GA_HitReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbility.h"

UYH_GA_HitReact::UYH_GA_HitReact()
{
	// This ability is instanced per execution.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// Ability tag - used to activate via TryActiveAbilitesByTag
	FGameplayTagContainer NewTags;
	NewTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.HitReact")));
	SetAssetTags(NewTags);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.HitReact"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UYH_GA_HitReact::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);




	//Ability plays the montage.
	UAnimMontage* AbilityMontage = Cast<UAnimMontage>(const_cast<UObject*>(TriggerEventData->OptionalObject.Get()));
	if (!AbilityMontage)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage);

	if (PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UYH_GA_HitReact::OnMontageEnd);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UYH_GA_HitReact::OnMontageEnd);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UYH_GA_HitReact::OnMontageEnd);
		PlayMontageTask->ReadyForActivation();
	}

}

void UYH_GA_HitReact::OnMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UYH_GA_HitReact::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicatedEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}