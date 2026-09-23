//Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Gun Attack Damage Effect (Thunder — Lightning Element)
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_EffectBase.h"
#include "YH_GE_GunDamage.generated.h"

/**
 * Instant damage applied to enemy Health on gun hit confirmation.
 * Base damage: 8.0 (lower than sword, higher fire rate compensates)
 * Scales with Maurice's AttackPower attribute post vertical slice.
 */
UCLASS()
class GASOLINE_API UYH_GE_GunDamage : public UGASO_EffectBase
{
	GENERATED_BODY()

public:
	UYH_GE_GunDamage();
	
};
