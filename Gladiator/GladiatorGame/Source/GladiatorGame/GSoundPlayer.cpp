// Fill out your copyright notice in the Description page of Project Settings.

#include "GSoundPlayer.h"


// Sets default values for this component's properties
UGSoundPlayer::UGSoundPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGSoundPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGSoundPlayer::AddSound(int32 p_index, USoundCue* p_newSound)
{

	if (p_newSound->IsValidLowLevelFast())
	{
		SoundSetUp[p_index]->SetSound(p_newSound);
	}
}

void UGSoundPlayer::PrePareSoundArray()
{
	int32 index = DefaultSoundClass.Num();
	for (int32 i = 0; i < index; ++i)
	{
		SoundSetUp.Add(NewObject<UAudioComponent>(this));
		SoundSetUp[i]->bAutoActivate = false;
		AddSound(i, DefaultSoundClass[i]);
	}
}

TArray<UAudioComponent*>& UGSoundPlayer::GetSounds()
{
	return SoundSetUp;
}

TArray<USoundCue*>& UGSoundPlayer::GetSoundInventory()
{
	return DefaultSoundClass;
}