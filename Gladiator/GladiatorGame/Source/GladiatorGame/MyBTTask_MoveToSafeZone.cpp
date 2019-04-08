// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_MoveToSafeZone.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GCharacter.h" 
#include "GEnemy.h"
#include "GEnemyCharacter.h"

EBTNodeResult::Type UMyBTTask_MoveToSafeZone::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		AGCharacter* Target = Cast<AGCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyPC->TargetKeyID));
		if (Target)
		{
			const int SafeDistanceToTarget = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Int>(EnemyPC->SafeDistanceToTargetKeyID);
			FVector distanceToPlayer = Target->GetTransform().GetLocation() - EnemyPC->GetCharacter()->GetTransform().GetLocation();
			if (static_cast<int>(distanceToPlayer.Size2D()) != SafeDistanceToTarget)
			{
				distanceToPlayer.Normalize();
				distanceToPlayer *= -1 * SafeDistanceToTarget;

				EnemyPC->SetFocus(Target);
				EnemyPC->MoveToLocation(Target->GetTransform().GetLocation() + distanceToPlayer, 10);

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
