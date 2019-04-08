// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInventory.generated.h"

class AGSword;
class AGShield;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLADIATORGAME_API UGInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGInventory();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<AGSword*> WeaponInventory;
	TArray<AGShield*> ArmorInventory;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TArray<TSubclassOf<AGSword>> DefaultInventoryClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TArray<TSubclassOf<AGShield>> DefaultInventoryShieldClass;
public:
	void AddWeapon(AGSword* p_newWeapon);
	void AddArmor(AGShield* p_newShield);
	void SpawnDefaultInventory();
	void SpawnDefaultShieldInventory();
	TArray<TSubclassOf<AGSword>>& GetDefaultWeaponInventory();
	TArray<TSubclassOf<AGShield>>& GetDefaultShieldInventory();
	TArray<AGSword*>& GetWeaponInventory();
	TArray<AGShield*>& GetArmorInventory();	
};
