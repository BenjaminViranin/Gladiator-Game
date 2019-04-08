// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GCharacter.h"
#include "GEnemy.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                      uint8* NodeMemory)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		AGCharacter* Player = Cast<AGCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyPC->TargetKeyID));

		if (Player)
		{
			EnemyPC->MoveToActor(Player, 30, true, true, true, nullptr, true);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
