// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackPlayer.h"
#include "Engine/Engine.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GEnemy.h"
#include "GEnemyCharacter.h"

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                      uint8* NodeMemory)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		AGEnemyCharacter* EnemyChar = Cast<AGEnemyCharacter>(EnemyPC->GetCharacter());
		if (EnemyChar)
		{
			if (!EnemyChar->IsAttacking())
			{
				EnemyChar->StartAttacking();
				return EBTNodeResult::Succeeded;
			}

			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(
				EnemyPC->InAttackPhaseKeyID, false);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(
				EnemyPC->InRangeToAttackKeyID, false);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(
				EnemyPC->InWaitPhaseKeyId, true);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
