// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTD_InAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTDecorator.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTD_InAttackRange::UBTD_InAttackRange()
{
	NodeName = TEXT("In Attack Range");
	bNotifyTick = true;
	TargetActorKey.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UBTD_InAttackRange, TargetActorKey), 
		AActor::StaticClass());
}

bool UBTD_InAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AICon = OwnerComp.GetAIOwner();
	const APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if(!Pawn|| !BB)return false;

	const AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)return false;


	return FVector::DistSquared(Pawn->GetActorLocation(), TargetActor->GetActorLocation()) 
			<= FMath::Square(AttackRange);

	
}

void UBTD_InAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);
}


