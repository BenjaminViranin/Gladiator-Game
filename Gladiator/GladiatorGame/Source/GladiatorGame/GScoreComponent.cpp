// Fill out your copyright notice in the Description page of Project Settings.

#include "GScoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GEnemyManager.h"


// Sets default values for this component's properties
UGScoreComponent::UGScoreComponent() : EnemyKilled(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGScoreComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int& UGScoreComponent::GetEnemiesKilled()
{
	return EnemyKilled;
}
