// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_EnemyAttack.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UBTT_EnemyAttack::UBTT_EnemyAttack()
{
	NodeName = TEXT("Enemy Attack");
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and pawn
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);

	if (!Pawn || !ASC || !AttackTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	// Fire the gameplay event - UYH_GA_EnemyAttack is triggered by YH.Combat.Event.EnemyAttack.* tags
	FGameplayEventData Payload;
	Payload.EventTag = AttackTag;
	Payload.Instigator = Pawn;
	Payload.Target = Pawn;

	const int32 TriggeredCount = ASC->HandleGameplayEvent(AttackTag, &Payload);

	return TriggeredCount > 0 ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
