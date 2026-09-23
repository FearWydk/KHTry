// Josh Brooks Copyright 2026	
// Elemental Action RPG — The Resonance
// Combat rule helpers implementation.


#include "Combat/YH_CombatStatics.h"



EYH_WeaponSubcategory UYH_CombatStatics::ResolveSubcategory(EYH_WeaponType Weapon, EYH_ElementType Element)
{
	switch (Element)
	{
	case EYH_ElementType::Earth:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::Iron;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::Steel;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::Meteorite;
		default:
			break;
		}
		break;

	case EYH_ElementType::Lightning:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::Thunderblade;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::ThunderGun;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::LightningBreath;

		default: break;
		}
		break;

	case EYH_ElementType::Fire:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::Flameblade;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::Firearm;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::FireFang;

		default: break;
		}

		break;

	case EYH_ElementType::Water:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::Tideblade;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::HydroShot;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::SharkBite;
		default: break;
		}

		break;

	case EYH_ElementType::Air:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::GaleSword;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::AirShot;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::HarpyTornado;
		default: break;
		}

		break;

	case EYH_ElementType::Light:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::ErasureBlade;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::WhiteOut;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::LanceRain;
		default: break;
		}
		break;
	case EYH_ElementType::Dark:
		switch (Weapon)
		{
		case EYH_WeaponType::Sword:
			return EYH_WeaponSubcategory::VoidSlash;
		case EYH_WeaponType::Gun:
			return EYH_WeaponSubcategory::DarkVortex;
		case EYH_WeaponType::Lance:
			return EYH_WeaponSubcategory::DarkRise;
		default: break;
		}
		break;

	default:
		break; // No valid subcategory if Element is None or unrecognized, even if Weapon is valid.
	}


	return EYH_WeaponSubcategory::None;
}


EYH_FinisherType UYH_CombatStatics::WeaponToFinisherType(EYH_WeaponType Weapon)
{
	switch (Weapon)
	{
	case EYH_WeaponType::Sword: 		return EYH_FinisherType::SwordFinisher;
	case EYH_WeaponType::Gun: 			return EYH_FinisherType::GunFinisher;
	case EYH_WeaponType::Lance: 		return EYH_FinisherType::LanceFinisher;
	default:							return EYH_FinisherType::None;
	}

}

FComboResult UYH_CombatStatics::EvaluateCombo(const TArray<FComboHistoryEntry>& ComboHistory)
{
	FComboResult Result; // Default values are all None, which is what we want for an empty or invalid combo.

	if (ComboHistory.Num() == 0)
	{
		return Result;
	}

	// For simplicity, this example implementation just looks at the most recent hit in the combo history to determine the finisher result.
	//
	// Phase 7 grows this out more:
	// - count distinct weapons in ComboHistory; if only one weapon used, finisher type = that weapon's finisher type, otherwise finisher type = "Mixed Finisher"
	// - tally elements for a real Dominant Element bonus (e.g. 3+ hits of the same element gives a bonus to the finisher, or even changes the finisher type entirely)
	const FComboHistoryEntry& DecidingHit = ComboHistory.Last();

	Result.FinisherType = WeaponToFinisherType(DecidingHit.Weapon);
	Result.DominantElement = DecidingHit.Element;
	Result.Subcategory = ResolveSubcategory(DecidingHit.Weapon, DecidingHit.Element);

	return Result;
}


