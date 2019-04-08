// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_QuitWaitingPhase.h"
#include "Engine/Engine.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GEnemy.h"
#include "GEnemyCharacter.h"

EBTNodeResult::Type UBTTask_QuitWaitingPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(
			EnemyPC->InWaitPhaseKeyId, false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}



