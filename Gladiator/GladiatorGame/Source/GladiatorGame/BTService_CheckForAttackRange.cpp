// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForAttackRange.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GCharacter.h"
#include "GEnemy.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

UBTService_CheckForAttackRange::UBTService_CheckForAttackRange()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		AGCharacter* Player = Cast<AGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player && EnemyPC->IsAlive())
		{
			if (FVector::Dist(EnemyPC->GetCharacter()->GetTransform().GetLocation(),
							Player->GetTransform().GetLocation()) < 150)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(EnemyPC->InRangeToAttackKeyID, true);
			}
		}
	}
}
