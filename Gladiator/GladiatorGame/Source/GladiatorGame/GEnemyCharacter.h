// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHealthComponent.h"
#include "GInventory.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "GEnemyCharacter.generated.h"


UCLASS()
class GLADIATORGAME_API AGEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* BotBehavior;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float SafeDistanceToEnemy;

protected:
	UPROPERTY(EditDefaultsOnly, category = "Inventory")
	FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, category = "Inventory")
	FName ArmorAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UGInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "BloodParticle")
	UParticleSystem* Particle;

	AGSword* CurrentWeapon;
	AGShield* CurrentArmor;
	bool IsDead;
public:
	// Sets default values for this character's properties
	AGEnemyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	void Die();
	void EquipWeapon(AGSword* p_weapon);
	void EquipShield(AGShield* p_shield);

	FName GetWeaponSocketName() const;
	FName GetArmorSocketName() const;
	UGHealthComponent* GetHealthComponent() const;

	void StartAttacking();
	void EndAttacking();
	void Bleed();
	bool IsAlive() const;
	void StayDead();
	void IncrementPlayerScore();
	void Lock();
	void Unlock();

	UFUNCTION(BlueprintImplementableEvent, Category = "LOCK")
	void LockEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "LOCK")
	void UnlockEvent();

	bool IsAttacking() const;
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UGHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);


protected:
	virtual void BeginPlay() override;

	void SpawnDefaultInventory();
	void SpawnDefaultShieldInventory();
	void SetCurrentWeapon(AGSword* p_newWeapon, AGSword* p_lastWeapon);
	void SetCurrentArmor(AGShield* p_newShield, AGShield* p_lastShield);

};
