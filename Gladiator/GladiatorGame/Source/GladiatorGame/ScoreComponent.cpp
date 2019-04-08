// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GEnemyManager.h"


// Sets default values for this component's properties
UScoreComponent::UScoreComponent() : EnemyKilled (0), EnemyAtGameStart (0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Buffer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGEnemyManager::StaticClass(), Buffer);

	EnemyAtGameStart = Cast<AGEnemyManager>(Buffer[0])->GetEnemyNum();
}


// Called every frame
void UScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UScoreComponent::GetEnemyKilled()
{
	TArray<AActor*> Buffer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGEnemyManager::StaticClass(), Buffer);

	EnemyKilled = EnemyAtGameStart - Cast<AGEnemyManager>(Buffer[0])->GetEnemyNum();

	return EnemyKilled;
}

