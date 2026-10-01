// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Weapon Component
// GASoline Plugin | Combat Module
//
// MESH TYPE NOTE:
// This ais for Weapons that are Static Meshes. If using this with Skeletal Meshes Weapons, change three things:
//   FYH_WeaponData::Mesh   UStaticMesh*          -> USkeletalMesh*
//   WeaponMeshComp         UStaticMeshComponent  -> USkeletalMeshComponent
//   SetStaticMesh(...)     -> SetSkeletalMesh(...)

#include "Combat/ActorComponent/YH_WeaponManager.h"
#include "AbilitySystemComponent.h"
#include "AbilitysystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Combat/ActorComponent/YH_WeaponManager.h"

UYH_WeaponManager::UYH_WeaponManager()
{
	//Switching is event driven; no per-frame work needed.
	PrimaryComponentTick.bCanEverTick = false;
}



void UYH_WeaponManager::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owner's ASC for loose-tag application.
	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Weapon Manager] No AbilitySystemComponent on %s - active-weapon tags will not be applied."),
			GetOwner() ? *GetOwner()->GetName() : TEXT("null owner"));
	}

	//Create the single weapon mesh that will be reskinned on each switch, attached to the hand.
	if (USkeletalMeshComponent* OwnerMesh = GetOwnerMesh())
	{
		WeaponMeshComp = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("ActiveWeaponMesh"));
		if (WeaponMeshComp)
		{
			// The GAS sphere trace handles hit detection - the mesh itself shouldn't collide.
			WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WeaponMeshComp->RegisterComponent();
			WeaponMeshComp->AttachToComponent(
				OwnerMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("hand_rWeaponSocket"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Weapon Manager] Owner %s has no skeletal mesh - weapon will not be visible."),
			GetOwner() ? *GetOwner()->GetName() : TEXT("null owner"));
	}

	//Clamp the starting index into range and apply the starting slot.
	if (EquippedWeapons.Num() > 0)
	{
		ActiveWeaponIndex = FMath::Clamp(ActiveWeaponIndex, 0, EquippedWeapons.Num() - 1);
		UpdateActiveWeapon(/*bBroadcast=*/true);
	}
}


void UYH_WeaponManager::SelectWeapon(int32 Index)
{
	//Blocked while a BlcokSwitchTag is Active
	if (!CanSwitch())
	{
		return;
	}
	//No-op if out of range or already equipped.
	if (!IsValidWeaponIndex(Index) || Index == ActiveWeaponIndex)
	{
		return;
	}
	ActiveWeaponIndex = Index;
	UpdateActiveWeapon();
}

void UYH_WeaponManager::CycleWeapon(int32 Direction)
{
	//Blocked while a BlockSwitchTag is active
	if (!CanSwitch())
	{
		return;
	}
	const int32 Num = EquippedWeapons.Num();
	if (Num <= 1 || Direction == 0)
	{
		return;
	}

	//(+Num) keeps the result non negative so reverse cycling (-1) warps correctly.
	ActiveWeaponIndex = (ActiveWeaponIndex + Direction + Num) % Num;
	UpdateActiveWeapon();
}

void UYH_WeaponManager::EquipWeapon(const FYH_WeaponData& WeaponData)
{
	//One slot per weapon type - re-equipping a type replaces its slot instead of duplicating it.
	int32 Index = FindWeaponIndexByType(WeaponData.WeaponType);
	if (Index != INDEX_NONE)
	{
		EquippedWeapons[Index] = WeaponData;
	}
	else
	{
		Index = EquippedWeapons.Add(WeaponData);
	}

	ActiveWeaponIndex = Index;
	UpdateActiveWeapon();
}

void UYH_WeaponManager::UnequipWeaponType(EYH_WeaponType WeaponType)
{
	const int32 Index = FindWeaponIndexByType(WeaponType);
	if (Index == INDEX_NONE)
	{
		return;
	}

	const bool bWasActive = (Index == ActiveWeaponIndex);
	EquippedWeapons.RemoveAt(Index);

	if (EquippedWeapons.Num() == 0)
	{
		ClearActiveWeapon();
		return;
	}

	if (bWasActive)
	{
		//Fall back to the slot that slid into this index (or the new last slot).
		ActiveWeaponIndex = FMath::Min(Index, EquippedWeapons.Num() - 1);
		UpdateActiveWeapon();
	}
	else if (Index < ActiveWeaponIndex)
	{
		//Active slot shifted down by one - same weapon, so no re-apply or broadcast needed.
		--ActiveWeaponIndex;
	}
}

EYH_WeaponType UYH_WeaponManager::GetActiveWeaponType() const
{
	return IsValidWeaponIndex(ActiveWeaponIndex)
		? EquippedWeapons[ActiveWeaponIndex].WeaponType
		: EYH_WeaponType::None;
}

EYH_ElementType UYH_WeaponManager::GetActiveElementType() const
{
	return IsValidWeaponIndex(ActiveWeaponIndex)
		? EquippedWeapons[ActiveWeaponIndex].ElementType
		: EYH_ElementType::None;
}

FGameplayTag UYH_WeaponManager::GetActiveWeaponTag() const
{
	return IsValidWeaponIndex(ActiveWeaponIndex)
		? EquippedWeapons[ActiveWeaponIndex].ActiveTag
		: FGameplayTag();
}

void UYH_WeaponManager::UpdateActiveWeapon(bool bBroadcast)
{
	if (!IsValidWeaponIndex(ActiveWeaponIndex))
	{
		return;
	}
	const FYH_WeaponData& Weapon = EquippedWeapons[ActiveWeaponIndex];

	//---Swap the active-weapon gameplay tag on the owner's ASC ---
	if (OwnerASC)
	{
		if (AppliedWeaponTag.IsValid())
		{
			OwnerASC->RemoveLooseGameplayTag(AppliedWeaponTag);
		}
		AppliedWeaponTag = Weapon.ActiveTag;
		if (AppliedWeaponTag.IsValid())
		{
			OwnerASC->AddLooseGameplayTag(AppliedWeaponTag);
		}
	}

	//--- Swap the visible mesh and re-attach to this weapon's socket---
	if (WeaponMeshComp)
	{
		WeaponMeshComp->SetStaticMesh(Weapon.Mesh); // Null Mesh Clears this.
		if (USkeletalMeshComponent* OwnerMesh = GetOwnerMesh())
		{
			WeaponMeshComp->AttachToComponent(
				OwnerMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				Weapon.AttachSocket);
		}
	}

	//---Notify listeners (HUD weapon indicator, etc.) ---
	if (bBroadcast)
	{
		OnWeaponSwitched.Broadcast(ActiveWeaponIndex, Weapon.WeaponType, Weapon.ElementType);
	}
}

void UYH_WeaponManager::ClearActiveWeapon(bool bBroadcast)
{
	ActiveWeaponIndex = -1;

	if (OwnerASC && AppliedWeaponTag.IsValid())
	{
		OwnerASC->RemoveLooseGameplayTag(AppliedWeaponTag);
	}
	AppliedWeaponTag = FGameplayTag();

	if (WeaponMeshComp)
	{
		WeaponMeshComp->SetStaticMesh(nullptr);
	}

	if (bBroadcast)
	{
		OnWeaponSwitched.Broadcast(ActiveWeaponIndex, EYH_WeaponType::None, EYH_ElementType::None);
	}
}

int32 UYH_WeaponManager::FindWeaponIndexByType(EYH_WeaponType WeaponType) const
{
	return EquippedWeapons.IndexOfByPredicate(
		[WeaponType](const FYH_WeaponData& Weapon) { return Weapon.WeaponType == WeaponType; });
}

bool UYH_WeaponManager::IsValidWeaponIndex(int32 Index) const
{
	return EquippedWeapons.IsValidIndex(Index);
}

bool UYH_WeaponManager::CanSwitch() const
{
	// Allowed unless the owner currently has one of the configured blocking tags.
	return !(OwnerASC
		&& BlockSwitchTags.Num() > 0
		&& OwnerASC->HasAnyMatchingGameplayTags(BlockSwitchTags));
}

USkeletalMeshComponent* UYH_WeaponManager::GetOwnerMesh() const
{
	if (const ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		return OwnerChar->GetMesh();
	}

	//Fallback: first skeletal mesh component on the owner (non-Character pawns).
	return GetOwner() ? GetOwner()->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
}


