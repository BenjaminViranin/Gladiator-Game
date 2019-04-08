// Fill out your copyright notice in the Description page of Project Settings.

#include "GModeBase.h"
#include "GHealthComponent.h"
#include "GCharacter.h"
#include "GEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AGModeBase::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			AGCharacter* player = Cast<AGCharacter>(MyPawn);
			UGHealthComponent* HealthComp = Cast<UGHealthComponent>(player->GetComponentByClass(UGHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
			else
			{
				GameOver(0);
			}
		}
		else if (PC && !PC->GetPawn())
		{
			GameOver(0);
		}
	}
}

void AGModeBase::CheckAnyAIAlive()
{
	int32 index = 0;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn && !TestPawn->IsPlayerControlled())
		{
			AGEnemyCharacter* enemy = Cast<AGEnemyCharacter>(TestPawn);
			if (enemy)
			{
				UGHealthComponent* HealthComp = Cast<UGHealthComponent>(enemy->GetComponentByClass(UGHealthComponent::StaticClass()));

				if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
				{
					++index;
					break;
				}
			}
		}
	}
	if (index == 0)
	{
		GameOver(1);
	}
}

void AGModeBase::GameOver(int32 index)
{
	SetGameState(EBattleState::GameOver);
	if(index == 0)
	{
		//SoundPlayer->GetSounds()[1]->Play();
		PlayerLose();
	}
	else
	{
		//SoundPlayer->GetSounds()[0]->Play();
		PlayerWin();
	}
}

void AGModeBase::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	//StartGame();
}

void AGModeBase::StartGame()
{
	SetGameState(EBattleState::Start);
	RespawnPlayer();
	SpawnEnemies();
}

void AGModeBase::RespawnPlayer()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void AGModeBase::SetGameState(EBattleState p_newState)
{
	AGStateBase* GameState = GetGameState<AGStateBase>();
	if (ensureAlways(GameState))
	{
		GameState->SetBattleState(p_newState);
	}
}

AGModeBase::AGModeBase()
{
	RestartingTime = 5.0f;
	GameStateClass = AGameStateBase::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	SoundPlayer = CreateDefaultSubobject<UGSoundPlayer>(TEXT("AUDIO"));
}

void AGModeBase::StartPlay()
{
	Super::StartPlay();
	StartGame();
	SoundPlayer->PrePareSoundArray();
}

void AGModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckAnyAIAlive();
	CheckAnyPlayerAlive();
}
