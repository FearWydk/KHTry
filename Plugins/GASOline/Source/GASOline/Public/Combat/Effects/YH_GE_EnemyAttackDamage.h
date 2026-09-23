// Josh Brooks Copyright 2026

// Elemental Action RPG — The Severance
// Enemy Attack Damage Effect (Base Wrath)
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_EffectBase.h"
#include "YH_GE_EnemyAttackDamage.generated.h"

/**
 * Instant damage applied to Maurice's Health when a Wrath enemy attacks.
 * Base damage: 15.0
 * Applied from enemy ASC targeting player ASC.
 */
UCLASS()
class GASOLINE_API UYH_GE_EnemyAttackDamage : public UGASO_EffectBase
{
	GENERATED_BODY()

public:
	UYH_GE_EnemyAttackDamage();
	
};
