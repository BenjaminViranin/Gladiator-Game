// Fill out your copyright notice in the Description page of Project Settings.

#include "GEnemyManager.h"
#include "GEnemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GScoreComponent.h"

// Sets default values
AGEnemyManager::AGEnemyManager()
	: IdAttacking(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AGEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentWaitDuration > 0)
		CurrentWaitDuration -= DeltaTime;

	if (Enemies.Num() > 0)
	{
		if (CurrentWaitDuration <= 0)
		{
			if (IdAttacking < 0)
			{
				IdAttacking = rand() % Enemies.Num();
				while (!Enemies[IdAttacking]->IsAlive())
				{
					Enemies.RemoveAt(IdAttacking);
					if (Enemies.Num() > 0)
						IdAttacking = rand() % Enemies.Num();
					else
						return;
				}

				Enemies[IdAttacking]->SetInAttackPhase(true);
			}
			else
			{
				if (Enemies[IdAttacking]->IsAlive())
				{
					if (!Enemies[IdAttacking]->IsInAttackPhase())
						IdAttacking = -1;
				}
				else
				{
					Enemies.RemoveAt(IdAttacking);
					IdAttacking = -1;
				}
				if (MaxWaitDuration >= MinWaitDuration && (MaxWaitDuration - MinWaitDuration) > 0)
					CurrentWaitDuration = rand() % (MaxWaitDuration - MinWaitDuration) + MinWaitDuration;
				else
					CurrentWaitDuration = MinWaitDuration;
			}
		}
	}
	else
	{
		GetEnemies();
	}
}

int AGEnemyManager::GetEnemyNum()
{
	return Enemies.Num();
}

// Called when the game starts or when spawned
void AGEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	GetEnemies();
}

void AGEnemyManager::GetEnemies()
{
	TArray<AActor*> Buffer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGEnemy::StaticClass(), Buffer);

	for (AActor* Actor : Buffer)
	{
		Enemies.Add(Cast<AGEnemy>(Actor));
	}
}


