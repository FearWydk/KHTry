// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "Combat/ActorComponent/YH_WeaponManager.h"
#include "EquipmentDef.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract, Const)
class KHTRY_API UEquipmentDef : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<AActor> EquipmentActorClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName AttachSocketName;

	// If true, this def represents a weapon: UEquipmentInst routes it to the character's
	// YH_WeaponManager (WeaponData below) instead of spawning EquipmentActorClass. Weapons don't
	// need their own actor - WeaponManager owns a single reskinned mesh for the active weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	bool bIsWeapon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "bIsWeapon"))
	FYH_WeaponData WeaponData;

};
