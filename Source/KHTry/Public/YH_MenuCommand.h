// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "YH_MenuCommand.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct KHTRY_API FYH_MenuCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	FGameplayTag CommandTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	float Cost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	bool bHasSubmenu = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	int32 ShortcutSlot = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Command")
	FLinearColor BackgroundColor;

};