// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentInst.h"

#include "Equipment/EquipmentDef.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Items/ItemInstance.h"
#include "Items/Fragments/ItemFragement_Equippable.h"
#include "Combat/ActorComponent/YH_WeaponManager.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEquipmentInst::Initialize(UItemInstance* ItemInst, ACharacter* Character)
{
	if (!ItemInst) return;
	
	SourceItemInstance = ItemInst;
	
	const UItemFragement_Equippable* EquippableFragment = Cast<UItemFragement_Equippable>(
		ItemInst->FindFragmentByClass(UItemFragement_Equippable::StaticClass()));
	
	if (!EquippableFragment) return;
	
	EquipmentDef = EquippableFragment->EquipmentDef;
	HandleEquipItem(Character);
}



void UEquipmentInst::HandleEquipItem(ACharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentInst::HandleEquipItem: null Character - caller didn't pass a valid owner."));
		return;
	}

	UEquipmentDef* DefinitionCDO = EquipmentDef.GetDefaultObject();

	if (!DefinitionCDO) return;

	// Weapons route to YH_WeaponManager instead of spawning a generic equipment actor - it owns
	// their visual (a single reskinned mesh component) and drives combat ability/tag selection.
	if (DefinitionCDO->bIsWeapon)
	{
		if (UYH_WeaponManager* WeaponManager = Character->FindComponentByClass<UYH_WeaponManager>())
		{
			WeaponManager->EquipWeapon(DefinitionCDO->WeaponData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UEquipmentInst::HandleEquipItem: %s has no YH_WeaponManager - cannot equip weapon."), *Character->GetName());
		}
		return;
	}

	if (!DefinitionCDO->EquipmentActorClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	SpawnedEquipmentActor = World->SpawnActor(DefinitionCDO->EquipmentActorClass);

	if (!SpawnedEquipmentActor) return;

	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (!CharacterMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentInst::HandleEquipItem: %s has no skeletal mesh to attach to."), *Character->GetName());
		return;
	}

	SpawnedEquipmentActor->AttachToComponent(
		CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DefinitionCDO->AttachSocketName);
	
	
}

void UEquipmentInst::HandleUnequipItem(ACharacter* Character)
{
	const UEquipmentDef* DefinitionCDO = EquipmentDef.GetDefaultObject();
	if (DefinitionCDO && DefinitionCDO->bIsWeapon)
	{
		if (UYH_WeaponManager* WeaponManager = Character ? Character->FindComponentByClass<UYH_WeaponManager>() : nullptr)
		{
			WeaponManager->UnequipWeaponType(DefinitionCDO->WeaponData.WeaponType);
		}
		return;
	}

	if (SpawnedEquipmentActor)
	{
		SpawnedEquipmentActor->Destroy();
		SpawnedEquipmentActor = nullptr;
	}
	
	
}
