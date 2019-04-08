// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GEnemy.h"
#include "GameFramework/Actor.h"
#include "GEnemyManager.generated.h"

UCLASS()
class GLADIATORGAME_API AGEnemyManager : public AActor
{
	GENERATED_BODY()

public:

protected:
	TArray<AGEnemy*> Enemies;
	int IdAttacking;

	UPROPERTY(EditAnywhere)
	int MinWaitDuration;
	UPROPERTY(EditAnywhere)
	int MaxWaitDuration;

	float CurrentWaitDuration;

public:	
	// Sets default values for this actor's properties
	AGEnemyManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetEnemyNum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetEnemies();
};
