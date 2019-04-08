// Fill out your copyright notice in the Description page of Project Settings.

#include "GEnemyLockSystem.h"
#include "GEnemyCharacter.h"
#include "GCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UGEnemyLockSystem::UGEnemyLockSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	IsLocked = false;
	MinimumDistanceToLock = 500;
	ClosestTargetDistance = 0;
	GotEnemies = false;
	// ...
}


// Called when the game starts
void UGEnemyLockSystem::BeginPlay()
{
	Super::BeginPlay();
	AActor* temp = GetOwner();
	if (temp)
	{
		Player = Cast<AGCharacter>(temp);
	}
	GetAllEnemies();
	// ...
	
}


void UGEnemyLockSystem::GetAllEnemies()
{
	TArray<AActor*> Buffer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGEnemyCharacter::StaticClass(), Buffer);

	for (int i = 0; i < Buffer.Num(); ++i)
	{
		Enemies.Add(Cast<AGEnemyCharacter>(Buffer[i]));
	}
}

// Called every frame
void UGEnemyLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsLocked)
	{
		UpdatePlayerRotation();
	}
	if (!GotEnemies && Enemies.Num() == 0)
	{
		GetAllEnemies();
		if (Enemies.Num() > 0)
			GotEnemies = true;
	}
	// ...
}

void UGEnemyLockSystem::CalculateClosestTargetDistance()
{
	if (NearestTarget)
	{
		NearestTarget = nullptr;
	}
	if (ClosestTargetDistance != 0)
	{
		ClosestTargetDistance = 0;
	}
	FVector2D EnemyPosition;
	FVector2D PlayerPosition;
	float Distance = 0;
	int Index = -1;
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		PlayerPosition.X = Cast<ACharacter>(MyOwner)->GetActorLocation().X;
		PlayerPosition.Y = Cast<ACharacter>(MyOwner)->GetActorLocation().Y;
	}

	for(int i = 0; i < Enemies.Num(); i++)
	{
		if (Enemies[i]->GetHealthComponent()->GetHealth() > 0)
		{
			EnemyPosition.X = Enemies[i]->GetActorLocation().X;
			EnemyPosition.Y = Enemies[i]->GetActorLocation().Y;
			Distance = PlayerPosition.Distance(PlayerPosition, EnemyPosition);
			if (Distance >= MinimumDistanceToLock)
			{
				if (ClosestTargetDistance == 0)
				{
					ClosestTargetDistance = Distance;
					Index = i;
				}
				else if (ClosestTargetDistance > Distance)
				{
					ClosestTargetDistance = Distance;
					Index = i;
				}
			}
		}
		else
		{
			Enemies.RemoveAt(i);
		}
	}
	if (Index != -1)
	{
		NearestTarget = Enemies[Index];
		Lock();
	}
}

void UGEnemyLockSystem::UpdatePlayerRotation()
{
	if (!NearestTarget->IsAlive())
	{
		NearestTarget->Unlock();
		int32 index = Enemies.Find(NearestTarget);
		Enemies.RemoveAt(index);
		UnLock();
		NearestTarget = nullptr;
	}
	if (Player && Player->GetHealthComponent()->GetHealth() > 0.f && NearestTarget)
	{
		Player->bUseControllerRotationPitch = true;
		Player->bUseControllerRotationYaw = true;
		Player->bUseControllerRotationRoll = true;
		Player->GetCameraBoom()->bUsePawnControlRotation = false;
		FVector CameraLocation = Player->GetCameraComponent()->GetComponentLocation();
		FVector EnemyLocation = NearestTarget->GetActorLocation();
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, EnemyLocation);
		FRotator PlayerRotation = Player->GetController()->GetControlRotation();
		float FinalYaw = UKismetMathLibrary::RInterpTo(PlayerRotation, Rotation, GetWorld()->GetDeltaSeconds(), 7.0f).Yaw;
		TargetRotation = FRotator(PlayerRotation.Pitch, Rotation.Yaw, PlayerRotation.Roll);
		TargetRotation.Normalize();
		Player->GetController()->SetControlRotation(TargetRotation);
	}
}

void UGEnemyLockSystem::Lock()
{
	if (!IsLocked)
	{
		IsLocked = true;
		if(NearestTarget)
			NearestTarget->Lock();
	}

}

void UGEnemyLockSystem::UnLock()
{
	if (IsLocked)
	{
		IsLocked = false;
		if (Player)
		{
			Player->bUseControllerRotationPitch = false;
			Player->bUseControllerRotationYaw = false;
			Player->bUseControllerRotationRoll = false;
			Player->GetCameraBoom()->bUsePawnControlRotation = true;
		}
		if(NearestTarget)
			NearestTarget->Unlock();
	}
}

void UGEnemyLockSystem::TargetNextEnemy()
{
	if (NearestTarget)
	{
		NearestTarget->Unlock();
		//NearestTarget should make it's highlight off
		int32 index = Enemies.Find(NearestTarget);
		NearestTarget = nullptr;
		if (index == Enemies.Num() - 1)
		{
			index = 0;
		}
		else
		{
			++index;
		}
		NearestTarget = Enemies[index];
		NearestTarget->Lock();
	}
}

void UGEnemyLockSystem::TargetPreviousEnemy()
{
	if (NearestTarget)
	{
		NearestTarget->Unlock();
		int32 index = Enemies.Find(NearestTarget);
		NearestTarget = nullptr;
		if (index == 0)
		{
			index = Enemies.Num() - 1;
		}
		else
		{
			--index;
		}
		NearestTarget = Enemies[index];
		NearestTarget->Lock();
	}
}

float& UGEnemyLockSystem::GetClosestTargetDistance()
{
	return ClosestTargetDistance;
}

float & UGEnemyLockSystem::GetMinimumDistanceToLock()
{
	return MinimumDistanceToLock;
}

bool & UGEnemyLockSystem::GetIsLocked()
{
	return IsLocked;
}

FRotator & UGEnemyLockSystem::GetTargetRotation()
{
	return TargetRotation;
}

AGEnemyCharacter* UGEnemyLockSystem::GetNearestTarget()
{
	return NearestTarget;
}