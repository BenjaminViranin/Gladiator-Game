// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GCharacter.h"
#include "GHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UGHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float FullHealth;
	
	virtual void NativeConstruct() override;
	
public:

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UGHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	float& GetCurrentHealth();
	float& GetFullHeath();
};
