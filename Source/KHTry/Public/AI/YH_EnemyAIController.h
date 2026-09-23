// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "YH_EnemyAIController.generated.h"



class UBehaviorTree;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class KHTRY_API AYH_EnemyAIController : public AAIController
{
	GENERATED_BODY()		


protected:


	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetActorKey = "TargetActor";

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;


	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	AYH_EnemyAIController();
};
