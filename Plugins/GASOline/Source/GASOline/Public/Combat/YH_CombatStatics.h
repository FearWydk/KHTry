// Josh Brooks Copyright 2026	
// Elemental Action RPG — The Resonance
// Combat rule helpers: Weapon+Element -> Subcategory, combo -> FComboResult
// GASoline Plugin | Combat Module
// Pure enum/struct logic only — no asset references, no actor state.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "YH_CombatTypes.h"
#include "YH_CombatStatics.generated.h"

/**
 * 
 */
UCLASS()
class GASOLINE_API UYH_CombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//Weapon + Element -> Subcategory. E.g. Earth + Sword -> Iron, Lightning + Gun -> Thunder Gun, etc.
	//Returns None if the combination is invalid (e.g. Element = None, or a future expansion weapon/element with no recipe yet).
	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	static EYH_WeaponSubcategory ResolveSubcategory(EYH_WeaponType Weapon, EYH_ElementType Element);

	// Weapon -> FinisherType. E.g. Sword -> Sword Finisher, Gun -> Gun Finisher, etc.
	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	static EYH_FinisherType WeaponToFinisherType(EYH_WeaponType Weapon);

	// Combo history -> Finisher result. This is where the finisher recipe lives, reading the combo history and returning which finisher to trigger.
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	static FComboResult EvaluateCombo(const TArray<FComboHistoryEntry>& ComboHistory);

};
