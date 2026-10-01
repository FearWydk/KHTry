// Elemental Action RPG — The Resonance
// Maurice (Cipher) Player Attribute Set
// KHTry Game Module | Combat

#pragma once

#include "CoreMinimal.h"
#include "Combat/YH_AttributeSet.h"
#include "YH_PlayerAttributeSet.generated.h"

/**
 * Attribute set for Maurice and all player-controlled Ciphers.
 * Everything shared with enemies (Strength/Magic/Defense/AttackPower/.../HitReact+Death
 * dispatch) lives on UYH_AttributeSet; this class is the place for player-only stats as they
 * come up. Focus was removed - it was standing in for Stamina, which UGASO_AttributeSet
 * already provides.
 */
UCLASS()
class KHTRY_API UYH_PlayerAttributeSet : public UYH_AttributeSet
{
	GENERATED_BODY()

public:
	UYH_PlayerAttributeSet();
};
