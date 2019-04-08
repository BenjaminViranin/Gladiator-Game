// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHealthComponent.h"
#include "GInventory.h"
#include "GEnemyLockSystem.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "GCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class AGSword;
class AGShield;

UCLASS()
class GLADIATORGAME_API AGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCharacter();
	USkeletalMeshComponent* GetPawnMesh() const;
	FName GetWeaponSocketName() const;
	FName GetArmorSocketName() const;
	UGHealthComponent* GetHealthComponent() const;

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float p_value);
	void StrafeLeft(float p_value);
	void StrafeRight(float p_value);
	void MoveBackward(float p_value);
	void MoveLeft(float p_value);
	void TargetNext();
	void TargetPrev();
	void TargetLock();
	void LockEnemy();
	void UnlockEnemy();
	void PitchInput(float p_val);
	void YawInput(float p_val);
	void MoveRight(float p_value);
	bool IsDead;
	FRotator TargetRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGEnemyLockSystem* LockSystem;

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
	void SetCurrentWeapon(AGSword* p_newWeapon, AGSword* p_lastWeapon);
	void SetCurrentArmor(AGShield* p_newShield, AGShield* p_lastShield);
	void StayDead();
public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void EquipWeapon(AGSword* p_weapon);
	void EquipShield(AGShield* p_shield);
	void WeaponAttack();
	void Bleed();
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UGHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	void EndWeaponAttack();
	void SpawnDefaultInventory();
	UCameraComponent* GetCameraComponent();
	USpringArmComponent* GetCameraBoom(); 
	void SpawnDefaultShieldInventory();
	void Die();

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FString Name;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	int32 Level;

	UFUNCTION(BlueprintImplementableEvent, Category = "LockOnTarget")
	void OnTarget();

	UFUNCTION(BlueprintImplementableEvent, Category = "LockOnTarget")
	void OffTarget();

};