// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentInst.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Equipment/EquipmentDef.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Items/ItemInstance.h"
#include "Items/Fragments/ItemFragement_Equippable.h"
#include "Combat/ActorComponent/YH_WeaponManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

void UEquipmentInst::Initialize(UItemInstance* ItemInst, ACharacter* Character, TSubclassOf<UGameplayEffect> EquipmentStats)
{
	if (!ItemInst) return;
	
	SourceItemInstance = ItemInst;
	
	const UItemFragement_Equippable* EquippableFragment = Cast<UItemFragement_Equippable>(
		ItemInst->FindFragmentByClass(UItemFragement_Equippable::StaticClass()));
	
	if (!EquippableFragment) return;
	
	EquipmentDef = EquippableFragment->EquipmentDef;
	HandleEquipItem(Character, EquipmentStats);
}



void UEquipmentInst::HandleEquipItem(ACharacter* Character, TSubclassOf<UGameplayEffect> EquipmentStats)
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
	}
	else if (DefinitionCDO->EquipmentActorClass)
	{
		if (UWorld* World = GetWorld())
		{
			SpawnedEquipmentActor = World->SpawnActor(DefinitionCDO->EquipmentActorClass);
		}

		if (SpawnedEquipmentActor)
		{
			if (USkeletalMeshComponent* CharacterMesh = Character->GetMesh())
			{
				SpawnedEquipmentActor->AttachToComponent(
					CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DefinitionCDO->AttachSocketName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UEquipmentInst::HandleEquipItem: %s has no skeletal mesh to attach to."), *Character->GetName());
			}
		}
	}

	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	if (OwnerASC && EquipmentStats && SourceItemInstance)
	{
		FGameplayEffectContextHandle EffectContext = OwnerASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(EquipmentStats, 1.0f, EffectContext);
		
		if (SpecHandle.IsValid())
		{
			for (const auto& Pair:SourceItemInstance->StatsMap)
			{
				FGameplayTag StatTag = Pair.Key;
				float StatValue = Pair.Value;
				
				SpecHandle.Data->SetSetByCallerMagnitude(StatTag, StatValue);
			}
			OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
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
