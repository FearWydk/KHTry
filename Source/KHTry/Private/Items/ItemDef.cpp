// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemDef.h"
#include "Items/Fragments/ItemFragment.h"
#include "Templates/SubclassOf.h"

const UItemFragment* UItemDef::FindFragmentByClass(const TSubclassOf<UItemDef> ItemClass, const TSubclassOf<UItemFragment> FragmentClass)
{
    if(ItemClass && FragmentClass)
    {
		const UItemDef* ItemCDO = GetDefault<UItemDef>(ItemClass);
        for(const TObjectPtr<UItemFragment>& Fragment : ItemCDO->ItemFragments)
        {
            if(Fragment && Fragment->IsA(FragmentClass))
            {
                return Fragment;
            }
        }
    }
    return nullptr;
}
