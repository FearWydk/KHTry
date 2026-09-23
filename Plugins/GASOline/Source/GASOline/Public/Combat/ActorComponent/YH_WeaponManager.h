// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Weapon Component — loadout, active weapon, and switching
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../YH_CombatTypes.h"
#include "GameplayTagContainer.h"
#include "YH_WeaponManager.generated.h"

class UAbilitySystemComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UStaticMesh;

/**
 * One equipped weapon slot. Configure the EquippedWeapons array on the
 * component instance in the editor.
 */

USTRUCT(BlueprintType)
struct FYH_WeaponData
{
	GENERATED_BODY()

	//Which weapon this slot represents. Drives attack-ability / montage lookup downstream.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EYH_WeaponType WeaponType = EYH_WeaponType::None;

	//The Set Element on the Weapon. Drives combo finishers/ montage lookup downstream.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EYH_ElementType ElementType = EYH_ElementType::None;

	//Visible mesh attached to the character's hand while this weapon is active.
	//NOTE: assumes a Static Mesh weapon. If Skeletal, see cpp comment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	// Socket on the characters's skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName AttachSocket = FName("hand_rWeaponSocket");

	// Loose gameplay tag for this weapon. Used for ability / montage lookup downstream.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FGameplayTag ActiveTag;

};

// Broadcast on every succesful weapon switch. Hook to UI, VFX, etc.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponSwitched, int32, NewIndex, EYH_WeaponType, NewWeapon, EYH_ElementType, NewElement);


//Single source of truth for the players weapon loadout.

UCLASS( ClassGroup=(YH), meta=(BlueprintSpawnableComponent))
class GASOLINE_API UYH_WeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYH_WeaponManager();

	//** Directly equip the weapon in the given slot. Ignored if the index is out of range. */
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void SelectWeapon(int32 Index);

	//** Cycle to the next weapon in the EquippedWeapons array by Direction (+1 forward, -1 back). Wraps to the beginning if at the end. /
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void CycleWeapon(int32 Direction);

	//** Convenice wrappers for direct input binding. */
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void NextWeapon() { CycleWeapon(1); }

	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void PreviousWeapon() { CycleWeapon(-1); }

	// --- Loadout (driven by the equipment system) ---

	//** Adds the weapon to the loadout (replacing any existing slot of the same WeaponType) and makes it active. */
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void EquipWeapon(const FYH_WeaponData& WeaponData);

	//** Removes the slot matching WeaponType. If it was active, falls back to another slot or unarmed. */
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Weapon")
	void UnequipWeaponType(EYH_WeaponType WeaponType);

	// --- Queries ---

	UFUNCTION(BlueprintPure, Category = "YH|Combat|Weapon")
	EYH_WeaponType GetActiveWeaponType() const;

	UFUNCTION(BlueprintPure, Category = "YH|Combat|Weapon")
	EYH_ElementType GetActiveElementType() const;

	UFUNCTION(BlueprintPure, Category = "YH|Combat|Weapon")
	int32 GetActiveWeaponIndex() const { return ActiveWeaponIndex; }

	UFUNCTION(BlueprintPure, Category = "YH|Combat|Weapon")
	FGameplayTag GetActiveWeaponTag() const;

	UPROPERTY(BlueprintAssignable, Category = "YH|Combat|Weapon")
	FOnWeaponSwitched OnWeaponSwitched;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Loadout. Configures on the component instance in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YH|Combat|Weapon")
	TArray<FYH_WeaponData> EquippedWeapons;

	//Current Slot
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "YH|Combat|Weapon")
	int32 ActiveWeaponIndex = -1; //-1 means unarmed

	// Switching is blocked while the owner's ASC has ANY of these tags. Configure on the component instance in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YH|Combat|Weapon")
	FGameplayTagContainer BlockSwitchTags;

private:
	// Applies the current slot: swaps the gameplay tag, swaps the mesh, broadcasts.
	void UpdateActiveWeapon(bool bBroadcast = true);

	// Returns to unarmed: removes the applied tag, clears the mesh, broadcasts index -1.
	void ClearActiveWeapon(bool bBroadcast = true);

	int32 FindWeaponIndexByType(EYH_WeaponType WeaponType) const;

	bool IsValidWeaponIndex(int32 Index) const;

	// False while the owner has any of the BlockSwitchTags, which can be used to prevent switching during certain states (e.g. mid-attack).
	bool CanSwitch() const;

	// Resolves the character's skeletal mesh that weapons attach to.
	USkeletalMeshComponent* GetOwnerMesh() const;


	// The single visible weapon mesh, created once at runtime and re-skinned on each switch.
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMeshComp = nullptr;

	// Cached owner ASC, for loose-tag application.
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;

	// The active-weapon tag currently applied to the ASC. Used to remove the tag on switch.
	FGameplayTag AppliedWeaponTag;
		
};
