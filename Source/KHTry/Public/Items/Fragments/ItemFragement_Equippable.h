// Josh Brooks Copyright 2026.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "Templates/SubclassOf.h"
#include "ItemFragement_Equippable.generated.h"

class UEquipmentDef;
/**
 * 
 */
UCLASS()
class KHTRY_API UItemFragement_Equippable : public UItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<UEquipmentDef> EquipmentDef;
};
