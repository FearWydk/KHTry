// Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Sword Attack Damage Effect (Iron — Earth Element)
// GASoline Plugin | Combat Module
#pragma once

#include "CoreMinimal.h"
#include "GASO_EffectBase.h"
#include "YH_GE_Damage.generated.h"

/**
 * Instant damage applied to enemy Health on sword hit confirmation.
 * Base damage: 10.0
 * Scales with Maurice's AttackPower attribute post vertical slice.
 */
UCLASS()
class GASOLINE_API UYH_GE_Damage : public UGASO_EffectBase
{
	GENERATED_BODY()
	
public:
	UYH_GE_Damage();
};
