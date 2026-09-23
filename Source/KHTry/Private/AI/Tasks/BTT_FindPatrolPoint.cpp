// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_FindPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GASO_CharacterBase.h"

UBTT_FindPatrolPoint::UBTT_FindPatrolPoint()
{
	NodeName = TEXT("Find Patrol Point");
	// Output key must be a Vector
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindPatrolPoint, BlackboardKey));
}

EBTNodeResult::Type UBTT_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and pawn
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
	AGASO_CharacterBase* Character = Cast<AGASO_CharacterBase>(Pawn);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	if (!Character || !BBComp || Character->WorldPatrolPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	const int32 Count = Character->WorldPatrolPoints.Num();
	const int32 Index = Character->CurrentPatrolIndex % Count;
	

	BBComp->SetValueAsVector(BlackboardKey.SelectedKeyName, Character->WorldPatrolPoints[Index]);
	

	Character->CurrentPatrolIndex = (Character->CurrentPatrolIndex + 1) % Count;
	

	return EBTNodeResult::Succeeded;
}