// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GEnemy.generated.h"

/** 
 *
 */
UCLASS()
class GLADIATORGAME_API AGEnemy : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

	bool InAttackPhase;

public:
	uint8 TargetKeyID;
	uint8 SafeDistanceToTargetKeyID;
	uint8 InAttackPhaseKeyID;
	uint8 InRangeToAttackKeyID;
	uint8 InWaitPhaseKeyId;

	AGEnemy();
	virtual void Possess(APawn* InPawn) override;

	bool IsAlive() const;

	bool IsInAttackPhase();
	void SetInAttackPhase(bool p_value);
};
