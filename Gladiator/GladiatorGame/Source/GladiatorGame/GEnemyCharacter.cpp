// Fill out your copyright notice in the Description page of Project Settings.

#include "GEnemyCharacter.h"
#include "Engine.h"
#include "GSword.h"
#include "GShield.h"
#include "GScoreComponent.h"

// Sets default values
AGEnemyCharacter::AGEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UGHealthComponent>(TEXT("HealthComp"));
	Inventory = CreateDefaultSubobject<UGInventory>(TEXT("Inventory"));
	IsDead = false;
}

void AGEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
	SpawnDefaultShieldInventory();
}

void AGEnemyCharacter::Die()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != nullptr)
	{
		Controller->UnPossess();
	}
	float DeathAnimDuration = PlayAnimMontage(DeathAnim);
	FTimerHandle TimerHandle_Dead;
	GetWorldTimerManager().SetTimer(TimerHandle_Dead, this, &AGEnemyCharacter::StayDead, DeathAnimDuration * 0.5f, false);
}

void AGEnemyCharacter::EquipWeapon(AGSword* p_weapon)
{
	if (p_weapon)
	{
		SetCurrentWeapon(p_weapon, CurrentWeapon);
	}
}

void AGEnemyCharacter::EquipShield(AGShield* p_shield)
{
	if (p_shield)
	{
		SetCurrentArmor(p_shield, CurrentArmor);
	}
}

FName AGEnemyCharacter::GetWeaponSocketName() const
{
	return WeaponAttachPoint;
}

FName AGEnemyCharacter::GetArmorSocketName() const
{
	return ArmorAttachPoint;
}

UGHealthComponent* AGEnemyCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

void AGEnemyCharacter::StartAttacking()
{
	if (CurrentWeapon && !CurrentWeapon->GetAttacking())
	{
		CurrentWeapon->StartAttack();
		CurrentWeapon->GetAttackDuration() = PlayAnimMontage(AttackAnim);

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AGEnemyCharacter::EndAttacking, CurrentWeapon->GetAttackDuration() * 0.5f, false);
	}
}

void AGEnemyCharacter::EndAttacking()
{
	if (CurrentWeapon && CurrentWeapon->GetAttacking())
	{
		CurrentWeapon->StopAttack();
	}
}

void AGEnemyCharacter::Bleed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, true);
}

bool AGEnemyCharacter::IsAlive() const
{
	return HealthComponent->GetHealth() > 0;
}

void AGEnemyCharacter::StayDead()
{
	GetMesh()->GlobalAnimRateScale = 0.0f;

	CurrentWeapon->SetActorEnableCollision(false);
}

void AGEnemyCharacter::IncrementPlayerScore()
{
	Cast<UGScoreComponent>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetComponentByClass(UGScoreComponent::StaticClass()))->GetEnemiesKilled()++;
}

void AGEnemyCharacter::Lock()
{
	LockEvent();
}

void AGEnemyCharacter::Unlock()
{
	UnlockEvent();
}

bool AGEnemyCharacter::IsAttacking() const
{
	return CurrentWeapon->GetAttacking();
}

void AGEnemyCharacter::OnHealthChanged(UGHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !IsDead)
	{
		IsDead = true;
		Die();

		IncrementPlayerScore();
	}
}


void AGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
		HealthComponent->OnHealthChanged.AddDynamic(this, &AGEnemyCharacter::OnHealthChanged);
}

void AGEnemyCharacter::SpawnDefaultInventory()
{
	Inventory->SpawnDefaultInventory();
	if (Inventory->GetWeaponInventory().Num() > 0)
	{
		EquipWeapon(Inventory->GetWeaponInventory()[0]);
	}
}

void AGEnemyCharacter::SpawnDefaultShieldInventory()
{
	Inventory->SpawnDefaultShieldInventory();
	if (Inventory->GetArmorInventory().Num() > 0)
	{
		EquipShield(Inventory->GetArmorInventory()[0]);
	}
}

void AGEnemyCharacter::SetCurrentWeapon(AGSword* p_newWeapon, AGSword* p_lastWeapon)
{
	AGSword* tempWeapon = nullptr;
	if (p_lastWeapon)
	{
		tempWeapon = p_lastWeapon;
	}
	else if (p_newWeapon != CurrentWeapon)
	{
		tempWeapon = CurrentWeapon;
	}
	if (tempWeapon)
	{
		tempWeapon->OnUnEquip();
	}
	CurrentWeapon = p_newWeapon;
	if (p_newWeapon)
	{
		p_newWeapon->SetOwner(this, true);
		p_newWeapon->OnEquip(p_lastWeapon);
	}
}

void AGEnemyCharacter::SetCurrentArmor(AGShield* p_newShield, AGShield* p_lastShield)
{
	AGShield* tempShield = nullptr;
	if (p_lastShield)
	{
		tempShield = p_lastShield;
	}
	else if (p_newShield != CurrentArmor)
	{
		tempShield = CurrentArmor;
	}
	if (tempShield)
	{
		tempShield->OnUnEquip();
	}
	CurrentArmor = p_newShield;
	if (p_newShield)
	{
		p_newShield->SetOwner(this, true);
		p_newShield->OnEquip(p_lastShield);
	}
}
