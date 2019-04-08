// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "GSoundPlayer.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLADIATORGAME_API UGSoundPlayer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGSoundPlayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TMap<FString, UAudioComponent*> SoundFiles;
	TArray<UAudioComponent*> SoundSetUp;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<USoundCue*> DefaultSoundClass;

public:	
	void AddSound(int32 p_index, USoundCue* p_newSound);
	void PrePareSoundArray();
	TArray<UAudioComponent*>& GetSounds();
	TArray<USoundCue*>& GetSoundInventory();
};