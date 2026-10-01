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
		UE_LOG(LogTemp, Warning, TEXT("No attack data found for tag: %s"),
			TriggerEventData ? *TriggerEventData->EventTag.ToString() : TEXT("NONE"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	// Cache on the ability instance (InstancedPerExecution) so PerformHitTrace can read it later off the HitCheck notify.
	CurrentAttackData = *AttackData;

	// Wait for AnimNotify HitCheck event from Blueprint, same as the player's sword attack.
	WaitHitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Notify.HitCheck")),
		nullptr,
		true);

	WaitHitCheckTask->EventReceived.AddDynamic(this, &UYH_GA_EnemyAttack::OnHitCheckReceived);
	WaitHitCheckTask->ReadyForActivation();

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		CurrentAttackData.AttackMontage);
	if (!PlayMontageTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create PlayMontageTask"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	PlayMontageTask->OnCompleted.AddDynamic(this, &UYH_GA_EnemyAttack::OnMontageEnd);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UYH_GA_EnemyAttack::OnMontageEnd);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UYH_GA_EnemyAttack::OnMontageEnd);
	PlayMontageTask->ReadyForActivation();
}

void UYH_GA_EnemyAttack::OnHitCheckReceived(FGameplayEventData Payload)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	PerformHitTrace(ActorInfo);
}

void UYH_GA_EnemyAttack::PerformHitTrace(const FGameplayAbilityActorInfo* ActorInfo)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor) return;

	UWorld* World = AvatarActor->GetWorld();
	if (!World) return;

	FVector TraceStart = AvatarActor->GetActorLocation();
	FVector TraceEnd = TraceStart + (AvatarActor->GetActorForwardVector() * CurrentAttackData.TraceRange);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	World->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(CurrentAttackData.TraceRadius),
		QueryParams
	);

	UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC) return;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;

		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC) continue;

		if (DamageEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(AvatarActor);

			FGameplayEffectSpecHandle SpecHandle =
				SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);

			if (SpecHandle.IsValid())
			{
				// Positive: YH_GE_Damage now targets the IncomingDamage meta attribute, which is
				// always a non-negative amount - UYH_AttributeSet does the subtraction from Health.
				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
					CurrentAttackData.Damage);

				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}

		// Only damage the first valid target hit per swing.
		break;
	}
}

void UYH_GA_EnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitHitCheckTask)
	{
		WaitHitCheckTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UYH_GA_EnemyAttack::OnMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
