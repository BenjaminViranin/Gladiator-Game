// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GSoundPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GSword.generated.h"

UCLASS()
class GLADIATORGAME_API AGSword : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGSword();
protected:
	ACharacter* Owner;
	bool isEnemy;
	bool Attacking;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	UGSoundPlayer* SoundPlayer;
	
	float AttackDuration;
public:	

	void SetOwner(ACharacter* p_owner, bool p_isEnemy = false);
	void AttachMeshToCharacter();
	void OnEquip(const AGSword* p_lastWeapon);
	void OnUnEquip();
	void StartAttack();
	void StopAttack();
	void SetAttacking();
	bool& GetAttacking();
	float& GetAttackDuration();

	UFUNCTION()
	virtual void OnSwordWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UBoxComponent* CollisionBox;
};