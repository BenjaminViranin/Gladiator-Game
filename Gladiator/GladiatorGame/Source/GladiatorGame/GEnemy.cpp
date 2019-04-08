// Fill out your copyright notice in the Description page of Project Settings.

#include "GEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GEnemyCharacter.h"

AGEnemy::AGEnemy()
	: InAttackPhase(false)
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AGEnemy::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AGEnemyCharacter* Char = Cast<AGEnemyCharacter>(InPawn);

	if (Char && Char->BotBehavior)
	{
		Blackboard->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);

		TargetKeyID = Blackboard->GetKeyID("Target");
		SafeDistanceToTargetKeyID = Blackboard->GetKeyID("SafeDistanceToTarget");
		InAttackPhaseKeyID = Blackboard->GetKeyID("InAttackPhase");
		InRangeToAttackKeyID = Blackboard->GetKeyID("InRangeToAttack");
		InWaitPhaseKeyId = Blackboard->GetKeyID("InWaitPhase");

		Blackboard->SetValue<UBlackboardKeyType_Int>(SafeDistanceToTargetKeyID, Char->SafeDistanceToEnemy);
		SetInAttackPhase(false);

		BehaviorComp->StartTree(*Char->BotBehavior);
	}
}

bool AGEnemy::IsAlive() const
{
	AGEnemyCharacter* EnemyCharac = Cast<AGEnemyCharacter>(GetCharacter());
	if (EnemyCharac)
	{
		return EnemyCharac->IsAlive();
	}

	return false;
}

bool AGEnemy::IsInAttackPhase()
{
	return (InAttackPhase = Blackboard->GetValue<UBlackboardKeyType_Bool>(InAttackPhaseKeyID));
}

void AGEnemy::SetInAttackPhase(bool p_value)
{
	InAttackPhase = p_value;
	Blackboard->SetValue<UBlackboardKeyType_Bool>(InAttackPhaseKeyID, p_value);
}
