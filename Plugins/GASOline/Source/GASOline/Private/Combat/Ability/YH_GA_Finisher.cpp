// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Ability/YH_GA_Finisher.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"

UYH_GA_Finisher::UYH_GA_Finisher()
{
	// This ability is instanced per execution.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	//Ability tag - used to activate via TryActiveAbilitesByTag
	FGameplayTagContainer NewTags;
	NewTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.Finisher")));
	SetAssetTags(NewTags);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.Finisher"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	// Block other attacks while this is active
	ActivationBlockedTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.GunAttack")));

	ActivationBlockedTags.AddTag(
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.SwordAttack")));

	
	
}

void UYH_GA_Finisher::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Wait for AnimNotify HitCheck event from Blueprint
	// Blueprint sends: Send Gamplay Event → YH.Combat.Event.FinisherHitCheck

	WaitHitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Notify.HitCheck")),
		nullptr,
		false);

	WaitHitCheckTask->EventReceived.AddDynamic(this, &UYH_GA_Finisher::OnFinisherHitCheckReceived);
	WaitHitCheckTask->ReadyForActivation();

	UAnimMontage* AbilityMontage = Cast<UAnimMontage>(const_cast<UObject*>(TriggerEventData->OptionalObject.Get()));
	if (!AbilityMontage)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}
	
		//Ability plays the montage.
	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage);

	if (PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UYH_GA_Finisher::OnMontageEnd);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UYH_GA_Finisher::OnMontageEnd);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UYH_GA_Finisher::OnMontageEnd);
		PlayMontageTask->ReadyForActivation();
	}

	
}



void UYH_GA_Finisher::OnFinisherHitCheckReceived(FGameplayEventData Payload)
{

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	PerformFinisherHitTrace(ActorInfo);

	
}

void UYH_GA_Finisher::OnMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UYH_GA_Finisher::PerformFinisherHitTrace(const FGameplayAbilityActorInfo* ActorInfo)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor) return;

	UWorld* World = AvatarActor->GetWorld();
	if (!World) return;

	// Trace from Maurice forward by HitTraceDistance
	FVector TraceStart = AvatarActor->GetActorLocation();
	FVector TraceEnd = TraceStart + (AvatarActor->GetActorForwardVector() * HitTraceDistance);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	// Sphere sweep for enemies using HitTraceRadius around Maurice's forward position in the shape of a sphere. Hits are sto
	World->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(FinisherHitTraceRadius),
		QueryParams
	);

	UAbilitySystemComponent* SourceASC =
		ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC) return;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;

		// Get the enemy's ASC
		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC) {

			continue;
		}

		// Apply Sword damage effect to enemy
		if (DamageEffectClass)
		{
			FGameplayEffectContextHandle EffectContext =
				SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(AvatarActor);

			FGameplayEffectSpecHandle SpecHandle =
				SourceASC->MakeOutgoingSpec(
					DamageEffectClass, 1.0f, EffectContext);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
					-DamageAmount);
				
				SourceASC->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(), TargetASC);
			}

		}

		//Send Hit.Confirmed event back to Blueprint
		//Blueprint uses this to progree combo counter
		FGameplayEventData HitConfirmedPayload;
		HitConfirmedPayload.Target = HitActor;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			AvatarActor,
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Hit.Confirmed")),
			HitConfirmedPayload
		);

		// Only damage the first valid enemy hit per swing

		break;

	}
}

void UYH_GA_Finisher::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (WaitHitCheckTask)
	{
		WaitHitCheckTask->EndTask();
	}
	
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}