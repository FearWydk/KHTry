// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_InAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class KHTRY_API UBTD_InAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_InAttackRange();

protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Condition")
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector TargetActorKey;
};
