// Fill out your copyright notice in the Description page of Project Settings.

#include "GHealthBarWidget.h"

void UGHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			AGCharacter* player = Cast<AGCharacter>(MyPawn);
			UGHealthComponent* HealthComp = Cast<UGHealthComponent>(player->GetComponentByClass(UGHealthComponent::StaticClass()));
			HealthComp->OnHealthChanged.AddDynamic(this, &UGHealthBarWidget::OnHealthChanged);
		}
	}
}

void UGHealthBarWidget::OnHealthChanged(UGHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	CurrentHealth = Health;
}

float & UGHealthBarWidget::GetCurrentHealth()
{
	return CurrentHealth;
}

float & UGHealthBarWidget::GetFullHeath()
{
	return FullHealth;
}
