// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsAlive.h"
#include "GEnemy.h"
#include "GEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

bool UBTDecorator_IsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		return EnemyPC->IsAlive();
	}

	return false;
}



