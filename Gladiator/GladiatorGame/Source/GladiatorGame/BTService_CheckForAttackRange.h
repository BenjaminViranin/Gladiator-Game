// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTService_CheckForAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckForAttackRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
