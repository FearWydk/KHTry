// Josh Brooks Copyright 2026

// Elemental Action RPG — The Resonance
// Instant Heal Effect
// GASoline Plugin | Combat Module
#pragma once

#include "CoreMinimal.h"
#include "GASO_EffectBase.h"
#include "YH_GE_Heal.generated.h"

/**
 * Instant heal applied to the target's Health, e.g. on potion use.
 * Magnitude is SetByCaller ("Data.Heal"), provided by the caller
 * (e.g. UItemAction_ApplyGameplayEffect on a potion's ItemFragment_Usable).
 *
 * Mirrors UYH_GE_Damage but adds to Health instead of subtracting.
 * For a lingering/periodic heal (save points, heal zones) use UYH_GE_HealOverTime instead -
 * this class is for a single, one-shot heal that does not keep applying.
 */
UCLASS()
class GASOLINE_API UYH_GE_Heal : public UGASO_EffectBase
{
	GENERATED_BODY()

public:
	UYH_GE_Heal();
};
