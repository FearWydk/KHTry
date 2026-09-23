// Elemental Action RPG — The Resonance
// Shared Combat Types: Enums and Structs
// GASoline Plugin | Combat Module
// All types are BlueprintType for KHTry Blueprint access

#pragma once

#include "CoreMinimal.h"
#include "YH_CombatTypes.generated.h"

// -------------------------------------------------------
// Weapon Types
// Defines the three weapons the player can wield.
// Will expand to 4 weapons in the future, but for now we have:1
// -------------------------------------------------------

UENUM(BlueprintType)
enum class EYH_WeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Sword UMETA(DisplayName = "Sword"),
	Gun UMETA(DisplayName = "Gun"),
	Lance UMETA(DisplayName = "Lance")
};

// -------------------------------------------------------
// Element Types
// The seven elements plus None for unelemented attacks.
// Weapon + Element = subcategory (e.g. Sword + Earth = Iron)
// -------------------------------------------------------
UENUM(BlueprintType)
enum class EYH_ElementType : uint8
{
	None UMETA(DisplayName = "None"),
	Earth UMETA(DisplayName = "Earth"),
	Fire UMETA(DisplayName = "Fire"),
	Water UMETA(DisplayName = "Water"),
	Air UMETA(DisplayName = "Air"),
	Lightning UMETA(DisplayName = "Lightning"),
	Light UMETA(DisplayName = "Light"),
	Dark UMETA(DisplayName = "Dark")
};

// -------------------------------------------------------
// Weapon Subcategory — derived from Weapon + Element
// Used for finisher naming and visual/audio cues
// -------------------------------------------------------
UENUM(BlueprintType)
enum class EYH_WeaponSubcategory : uint8
{
	None UMETA(DisplayName = "None"),
	// Earth
	Iron UMETA(DisplayName = "Iron"), // Earth + Sword
	Steel UMETA(DisplayName = "Steel"), // Earth + Gun
	Meteorite UMETA(DisplayName = "Meteorite"), // Earth + Lance

	// Lightning
	Thunderblade UMETA(DisplayName = "Thunderblade"), // Lightning + Sword
	ThunderGun UMETA(DisplayName = "Thunder Gun"), // Lightning + Gun
	LightningBreath UMETA(DisplayName = "Lightning Breath"), // Lightning + Lance

	// Fire
	Flameblade UMETA(DisplayName = "Flameblade"), // Fire + Sword
	Firearm UMETA(DisplayName = "Firearm"), // Fire + Gun
	FireFang UMETA(DisplayName = "Fire Fang"), // Fire + Lance

	// Water
	Tideblade UMETA(DisplayName = "Tideblade"), // Water + Sword
	HydroShot UMETA(DisplayName = "Hydro Shot"), // Water + Gun
	SharkBite UMETA(DisplayName = "Shark Bite"), // Water + Lance

	// Air
	GaleSword UMETA(DisplayName = "Gale Sword"), // Air + Sword
	AirShot UMETA(DisplayName = "Air Shot"), // Air + Gun
	HarpyTornado UMETA(DisplayName = "Harpy Tornado"), // Air + Lance

	// Light
	ErasureBlade UMETA(DisplayName = "Erasure Blade"), // Light + Sword
	WhiteOut UMETA(DisplayName = "White Out"), // Light + Gun
	LanceRain UMETA(DisplayName = "Lance Rain"), // Light + Lance

	// Dark
	VoidSlash UMETA(DisplayName = "Void Slash"), // Dark + Sword
	DarkVortex UMETA(DisplayName = "Dark Vortex"), // Dark + Gun
	DarkRise UMETA(DisplayName = "Dark Rise") // Dark + Lance
};

// -------------------------------------------------------
// Finisher Type
// Determined by the recipe evaluator at ComboCount == 3
// -------------------------------------------------------
UENUM(BlueprintType)
enum class EYH_FinisherType : uint8
{
	None UMETA(DisplayName = "None"),
	SwordFinisher UMETA(DisplayName = "Sword Finisher"), //Iron Cleave (verticle slice)
	GunFinisher UMETA(DisplayName = "Gun Finisher"), //Thunder Burst (verticle slice)
	LanceFinisher UMETA(DisplayName = "Lance Finisher"),//Meteor Strike (overhead strike)
	MixedFinisher UMETA(DisplayName = "Mixed Finisher") // For combos with multiple weapons

};

// -------------------------------------------------------
// Combo History Entry
// Tracks each hit in the current combo chain.
// Weapon + Element allows future magic injection into combos.
// e.g. Sword + Fire = Flame Slash finisher instead of Iron Cleave
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct FComboHistoryEntry
{
	
	GENERATED_BODY()

	// Which weapon landed this hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Combo")
	EYH_WeaponType Weapon = EYH_WeaponType::None;

	// Which element was active during this hit
	// None = no element injected (standard physical hit)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Combo")
	EYH_ElementType Element = EYH_ElementType::None;
};

// -------------------------------------------------------
// Combo Result
// Output of the recipe evaluator — passed to finisher ability
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct FComboResult
{
	GENERATED_BODY()
	// Which finisher ability to trigger
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Combo")
	EYH_FinisherType FinisherType = EYH_FinisherType::None;

	// The dominant element across the combo history
	// Used for visual/audio cue selection on the finisher
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Combo")
	EYH_ElementType DominantElement = EYH_ElementType::None;

	//The weapon subcategory for this finisher 
	// Iron Cleave, Thunder Burst, Meteor Strike, etc. (used for visual/audio cues)
	UPROPERTY(BlueprintReadWrite, Category = "Combat|Combo")
	EYH_WeaponSubcategory Subcategory = EYH_WeaponSubcategory::None;
};