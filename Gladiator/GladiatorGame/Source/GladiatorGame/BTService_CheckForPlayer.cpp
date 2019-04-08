// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GCharacter.h"
#include "GEnemy.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGEnemy* EnemyPC = Cast<AGEnemy>(OwnerComp.GetAIOwner());
	if (EnemyPC)
	{
		AGCharacter* Player = Cast<AGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyPC->TargetKeyID, Player);
		}
	}
}
