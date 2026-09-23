// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/YH_EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"	
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AYH_EnemyAIController::AYH_EnemyAIController()
{
	UAIPerceptionComponent* Perception =
		CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")); //Perception component is created in the constructor of the AIController class
	SetPerceptionComponent(*Perception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);

	//No Team affiliation, all actors are enemies
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	Perception->ConfigureSense(*SightConfig);
	Perception->SetDominantSense(SightConfig->GetSenseImplementation());
	Perception->OnTargetPerceptionUpdated.AddDynamic
		(this, &AYH_EnemyAIController::OnTargetPerceptionUpdated);
}

void AYH_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: no BehaviorTree assigned"), *GetName());
		return;
	}

	RunBehaviorTree(BehaviorTree);

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			BB->SetValueAsObject(TargetActorKey, PlayerPawn);
			SetFocus(PlayerPawn);
			UE_LOG(LogTemp, Warning, TEXT("Set TargetActor=%s"), *GetNameSafe(PlayerPawn));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Blackboard is null - BT didn't start"));
	}
	
}

void AYH_EnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Perception: %s sensed = %d"), *GetNameSafe(Actor),Stimulus.WasSuccessfullySensed());

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)return;

	
	if (Stimulus.WasSuccessfullySensed())
	{
		BB->SetValueAsObject(TargetActorKey, Actor);
			SetFocus(Actor);
			UE_LOG(LogTemp, Warning, TEXT("Set TargetActor=%s"), *GetNameSafe(Actor));
	}
	else
	{
		BB->ClearValue(TargetActorKey);
		ClearFocus(EAIFocusPriority::Gameplay);
		UE_LOG(LogTemp, Warning, TEXT("Cleared TargetActor"));
	}
	
}
