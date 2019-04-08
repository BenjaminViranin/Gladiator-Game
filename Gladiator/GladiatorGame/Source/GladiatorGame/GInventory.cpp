// Fill out your copyright notice in the Description page of Project Settings.

#include "GInventory.h"
#include "GSword.h"
#include "GShield.h"

// Sets default values for this component's properties
UGInventory::UGInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UGInventory::AddWeapon(AGSword* p_newWeapon)
{
	if (p_newWeapon)
	{
		WeaponInventory.AddUnique(p_newWeapon);
	}
}

void UGInventory::AddArmor(AGShield * p_newShield)
{
	if (p_newShield)
	{
		ArmorInventory.AddUnique(p_newShield);
	}
}

void UGInventory::SpawnDefaultInventory()
{
	int32 NumOfWeapons = DefaultInventoryClass.Num();
	for (int32 i = 0; i < NumOfWeapons; ++i)
	{
		if (DefaultInventoryClass[i])
		{
			FActorSpawnParameters SpawnInfo;
			AGSword* NewWeapon = GetWorld()->SpawnActor<AGSword>(DefaultInventoryClass[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}
}

void UGInventory::SpawnDefaultShieldInventory()
{
	int32 NumOfShield = DefaultInventoryShieldClass.Num();
	for (int32 i = 0; i < NumOfShield; ++i)
	{
		if (DefaultInventoryShieldClass[i])
		{
			FActorSpawnParameters SpawnInfo;
			AGShield* NewShield = GetWorld()->SpawnActor<AGShield>(DefaultInventoryShieldClass[i], SpawnInfo);
			AddArmor(NewShield);
		}
	}
}

TArray<TSubclassOf<AGSword>>& UGInventory::GetDefaultWeaponInventory()
{
	return DefaultInventoryClass;
}

TArray<TSubclassOf<AGShield>>& UGInventory::GetDefaultShieldInventory()
{
	return DefaultInventoryShieldClass;
}

TArray<AGSword*>& UGInventory::GetWeaponInventory()
{
	return WeaponInventory;
}

TArray<AGShield*>& UGInventory::GetArmorInventory()
{
	return ArmorInventory;
}