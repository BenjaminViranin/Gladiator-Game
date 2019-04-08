// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h" 
#include "Runtime/UMG/Public/UMGStyle.h" 
#include "Runtime/UMG/Public/Slate/SObjectWidget.h" 
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GPlayerHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UGPlayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetName") 
	FString WidgetName;
};