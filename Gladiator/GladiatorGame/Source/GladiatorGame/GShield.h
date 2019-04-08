// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GShield.generated.h"

UCLASS()
class GLADIATORGAME_API AGShield : public AActor
{
	GENERATED_BODY()

protected:
	ACharacter * Owner;
	bool isEnemy;

public:	
	// Sets default values for this actor's properties
	AGShield();
	void SetOwner(ACharacter* p_owner, bool p_isEnemy = false);
	void AttachMeshToCharacter();
	void OnEquip(const AGShield* p_lastShield);
	void OnUnEquip();
	UFUNCTION()
	virtual void OnShieldBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UBoxComponent* CollisionBox;
};
