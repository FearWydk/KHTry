// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"


class UItemFragment;
class UItemDef;
/**
 * 
 */
UCLASS(BlueprintType)
class KHTRY_API UItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Display")
	TSubclassOf<UItemDef> ItemDefinition;
	
	UPROPERTY(BlueprintReadOnly, Category = "Display")
	TMap<FGameplayTag, float> StatsMap;
		
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Stats", meta = (Categories = "Item.Stat"))
	float GetStatValue(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintCallable, Category = "Item Stats",meta = (Categories = "Item.Stat"))
	void SetStatValue(FGameplayTag StatTag, float StatValue);
	
	UFUNCTION(BlueprintCallable, Category = "Item Instance",meta = (AutoCreateRefTerm = "InitStats"))
	void Initialize(TSubclassOf<UItemDef> ItemDef, const TMap<FGameplayTag, float>& InitStats);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "FragmentClass"), Category = "ItemDef")
	const UItemFragment* FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass);
};
