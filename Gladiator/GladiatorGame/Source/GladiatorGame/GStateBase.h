// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GStateBase.generated.h"

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	Start,
	GameOver,
};

/**
*
*/
UCLASS()
class GLADIATORGAME_API AGStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GameState")
		EBattleState GameState;

public:
	void SetBattleState(EBattleState p_newState);

	UFUNCTION(BlueprintCallable, Category = "GameState")
	EBattleState GetBattleState();
};