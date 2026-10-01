// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// KHTry Game Module | Combat

#include "Combat/YH_EnemyAttributeSet.h"

UYH_EnemyAttributeSet::UYH_EnemyAttributeSet()
{
	// Enemies hit harder than the baseline AttackPower set on UYH_AttributeSet.
	InitAttackPower(15.f);
}
