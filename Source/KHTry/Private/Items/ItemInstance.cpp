// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemInstance.h"

#include "Items/ItemDef.h"
#include "Items/Fragments/ItemFragment.h"

float UItemInstance::GetStatValue(FGameplayTag StatTag)
{
	return StatsMap.FindRef(StatTag);
}
void UItemInstance::SetStatValue(FGameplayTag StatTag, float StatValue)
{
	StatsMap.Add(StatTag, StatValue);
}


void UItemInstance::Initialize(TSubclassOf<UItemDef> ItemDef,const TMap<FGameplayTag, float>& InitStats)
{
	if (!ItemDef) return;
	
	ItemDefinition = ItemDef;
	
	 UItemDef* ItemCDO = ItemDef.GetDefaultObject();
	
	for(const TObjectPtr<UItemFragment>& Fragment : ItemCDO->ItemFragments)
	{
		Fragment->OnInstanceCreated(this);
	}
	
	if (!InitStats.IsEmpty())
	{
		StatsMap = InitStats;
	}
}

const UItemFragment* UItemInstance::FindFragmentByClass(const TSubclassOf<UItemFragment> FragmentClass)
{
	if (!FragmentClass) return nullptr;
	
	UItemDef* ItemCDO = ItemDefinition.GetDefaultObject();
	
	return ItemCDO->FindFragmentByClass(ItemDefinition,FragmentClass);
}


	
	
