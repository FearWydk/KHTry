// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Fragments/ItemFragment.h"
#include "ItemFragment_WorldActor.generated.h"

/**
 * 
 */
UCLASS()
class KHTRY_API UItemFragment_WorldActor : public UItemFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Options")
	bool bCanBeDropped = true;

};

