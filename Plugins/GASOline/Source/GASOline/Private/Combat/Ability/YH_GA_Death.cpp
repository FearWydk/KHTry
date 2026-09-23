// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Ability/YH_GA_Death.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

UYH_GA_Death::UYH_GA_Death()
{
	// This ability is instanced per execution.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	//Ability tag - used to activate via TryActivAbilitesByTag
	FGameplayTagContainer NewTags;
	NewTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.Death")));
	SetAssetTags(NewTags);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.Death"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);


}

void UYH_GA_Death::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo,  ActivationInfo, TriggerEventData);

	if (!DeathMontage)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if(AvatarActor)
	{

		AvatarActor->SetActorEnableCollision(false);
		
		
			ACharacter* Char = Cast<ACharacter>(AvatarActor);
			if(Char&&Char->GetCharacterMovement())
			{
				Char->GetCharacterMovement()->DisableMovement();
			}
		
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DeathMontage);
	
	if (PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UYH_GA_Death::OnMontageEnd);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UYH_GA_Death::OnMontageEnd);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UYH_GA_Death::OnMontageEnd);
		PlayMontageTask->ReadyForActivation();
	}

}

void UYH_GA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AActor* A = GetAvatarActorFromActorInfo())
	{
		A->Destroy();
	}
	
}

void UYH_GA_Death::OnMontageEnd()
{
	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 10.0f);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
	
}

