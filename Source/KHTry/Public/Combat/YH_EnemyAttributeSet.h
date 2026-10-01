// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// KHTry Game Module | Combat

#pragma once

#include "CoreMinimal.h"
#include "Combat/YH_AttributeSet.h"
#include "YH_EnemyAttributeSet.generated.h"

/**
 * Attribute set for all Wrath enemies (Light Wrath, Dark Wrath).
 * Everything shared with the player (Strength/Magic/Defense/AttackPower/.../HitReact+Death
 * dispatch) lives on UYH_AttributeSet; this class is the place for enemy-only stats as they
 * come up (e.g. aggro range, loot tier).
 */
UCLASS()
class KHTRY_API UYH_EnemyAttributeSet : public UYH_AttributeSet
{
	GENERATED_BODY()

public:
	UYH_EnemyAttributeSet();
};
