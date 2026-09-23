// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "EquipmentInst.generated.h"

class AActor;
class ACharacter;
class UItemInstance;
class UEquipmentDef;
/**
 * 
 */
UCLASS(Blueprintable)
class KHTRY_API UEquipmentInst : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UEquipmentDef> EquipmentDef;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemInstance> SourceItemInstance;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> SpawnedEquipmentActor;
	
	UFUNCTION(BlueprintCallable)
	void Initialize(UItemInstance* ItemInst, ACharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void HandleEquipItem(ACharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void HandleUnequipItem(ACharacter* Character);
};
