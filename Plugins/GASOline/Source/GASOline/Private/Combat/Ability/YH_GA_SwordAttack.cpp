//Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Maurice Sword Attack Ability
// GASoline Plugin | Combat Module


#include "Combat/Ability/YH_GA_SwordAttack.h"
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



UYH_GA_SwordAttack::UYH_GA_SwordAttack()
{
		// This ability is instanced per execution.
		InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

		// Ability tag - used to activate via TryActiveAbilitesByTag
		FGameplayTagContainer NewTags;
		NewTags.AddTag(
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.SwordAttack")));
		SetAssetTags(NewTags);

		// Block other attacks while this is active
		ActivationBlockedTags.AddTag(
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.GunAttack")));

		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.SwordAttack"));
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
}

void UYH_GA_SwordAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	

	// Wait for AnimNotify HitCheck event from Blueprint
	// Blueprint sends: Send Gamplay Event → YH.Combat.Event.HitCheck

	WaitHitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Notify.HitCheck")),
		nullptr,
		true);

	WaitHitCheckTask->EventReceived.AddDynamic(this, &UYH_GA_SwordAttack::OnHitCheckReceived);
	WaitHitCheckTask->ReadyForActivation();
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
		PlayMontageTask->OnCompleted.AddDynamic(this, &UYH_GA_SwordAttack::OnMontageEnd);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UYH_GA_SwordAttack::OnMontageEnd);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UYH_GA_SwordAttack::OnMontageEnd);
		PlayMontageTask->ReadyForActivation();
	}

}

void UYH_GA_SwordAttack::OnHitCheckReceived(FGameplayEventData Payload)
{

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	PerformHitTrace(ActorInfo);

	
}

void UYH_GA_SwordAttack::PerformHitTrace(const FGameplayAbilityActorInfo* ActorInfo)
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
		FCollisionShape::MakeSphere(HitTraceRadius),
		QueryParams
	);

	UAbilitySystemComponent* SourceASC =
		ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC) return;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;
		UE_LOG(LogTemp, Display, TEXT("Hit Actor is valid"));
		// Get the enemy's ASC
		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC) {
			
			continue;
		}
		UE_LOG(LogTemp, Display, TEXT("Hit Target ASC is valid"));
		// Apply Sword damage effect to enemy
		if (DamageEffectClass)
		{
			UE_LOG(LogTemp, Display, TEXT("DamageEffectClass Valid"));
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
				
				
				UE_LOG(LogTemp, Display, TEXT("DamageAmount is valid"));

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

void UYH_GA_SwordAttack::OnMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false,true);
}

void UYH_GA_SwordAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicatedEndAbility,
	bool bWasCancelled)
{
	if (WaitHitCheckTask)
	{
		WaitHitCheckTask->EndTask();
	}
	

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}