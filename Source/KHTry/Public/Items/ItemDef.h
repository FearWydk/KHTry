// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "ItemDef.generated.h"

class UItemFragment;

UENUM(Blueprintable,BlueprintType)
enum class EItemRarity : uint8
{
	None,
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Atheral
};
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType, Abstract, Const)
class KHTRY_API UItemDef : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Display")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Display")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Display")
	EItemRarity Rarity = EItemRarity::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UItemFragment>> ItemFragments;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DeterminesOutputType = "FragmentClass"), Category = "ItemDef")
	static const UItemFragment* FindFragmentByClass(const TSubclassOf<UItemDef> ItemClass, const TSubclassOf<UItemFragment> FragmentClass);
};
