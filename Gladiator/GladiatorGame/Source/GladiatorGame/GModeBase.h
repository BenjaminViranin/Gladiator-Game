// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GStateBase.h"
#include "GSoundPlayer.h"
#include "TimerManager.h"
#include "GModeBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWin);
UCLASS()
class GLADIATORGAME_API AGModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_Restart;
	void CheckAnyPlayerAlive();
	void CheckAnyAIAlive();
	void GameOver(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void Restart();
	void StartGame();
	void RespawnPlayer();
	void SetGameState(EBattleState p_NewState);
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnEnemies();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void PlayerWin();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void PlayerLose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Restart")
	float RestartingTime;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	UGSoundPlayer* SoundPlayer;

public:
	AGModeBase();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameWin OnGameWin;
};