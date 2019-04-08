// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GEnemyLockSystem.generated.h"

class AGEnemyCharacter;
class AGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLADIATORGAME_API UGEnemyLockSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGEnemyLockSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<AGEnemyCharacter*> Enemies;
	AGEnemyCharacter* NearestTarget;
	AGCharacter* Player;
	float ClosestTargetDistance;
	float MinimumDistanceToLock;
	bool IsLocked;
	bool GotEnemies;
	FRotator TargetRotation;
public:	
	// Called every frame
	void GetAllEnemies();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CalculateClosestTargetDistance();
	void UpdatePlayerRotation();
	void Lock();
	void UnLock();
	void TargetNextEnemy();
	void TargetPreviousEnemy();
	float& GetClosestTargetDistance();
	float& GetMinimumDistanceToLock();
	UFUNCTION(BlueprintCallable, Category = "ISLOCKED")
	bool& GetIsLocked();
	FRotator& GetTargetRotation();
	AGEnemyCharacter* GetNearestTarget();
};