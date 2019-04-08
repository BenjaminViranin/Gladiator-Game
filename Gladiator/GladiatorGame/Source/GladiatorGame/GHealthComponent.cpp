// Fill out your copyright notice in the Description page of Project Settings.

#include "GHealthComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
UGHealthComponent::UGHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHealth = 5;
	IsDead = false;
}

// Called when the game starts
void UGHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UGHealthComponent::HandleTakeAnyDamage);
	}
	Health = DefaultHealth;
}

void UGHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || CurrentInvulnerability > 0)
	{
		return;
	}

	//Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	Health -= Damage;
	IsDead = Health <= 0.0f;
	CurrentInvulnerability = InvulnerabilityDuration;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UGHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentInvulnerability > 0)
		CurrentInvulnerability -= DeltaTime;
}


float UGHealthComponent::GetHealth() const
{
	return Health;
}