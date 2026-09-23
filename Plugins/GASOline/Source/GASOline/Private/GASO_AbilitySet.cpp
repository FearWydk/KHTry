// Fill out your copyright notice in the Description page of Project Settings.


#include "GASO_AbilitySet.h"
#include "Abilities/GameplayAbility.h"
#include "GASO_ASC.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GASO_AbilitySet)


void FGASO_AbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& SpecHandle)
{
	if (SpecHandle.IsValid())
	{
		AbilitySpecHandles.Add(SpecHandle);
	}
}

void FGASO_AbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& EffectHandle)
{
	if (EffectHandle.IsValid())
	{
		GameplayEffectHandles.Add(EffectHandle);
	}
}

void FGASO_AbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FGASO_AbilitySet_GrantedHandles::TakeFromAbilitySystem(UGASO_ASC* GASOASC)
{
	check(GASOASC);

	if (!GASOASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to grant/remove handles from the ASC
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			GASOASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			GASOASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		GASOASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UGASO_AbilitySet::UGASO_AbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGASO_AbilitySet::GiveToAbilitySystem(UGASO_ASC* GASOASC, FGASO_AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(GASOASC);

	if (!GASOASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to grant/remove handles from the ASC
		return;
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); SetIndex++)
	{
		const FGASO_AbilitySet_AttributeSet& AttributeSetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(AttributeSetToGrant.AttributeSet))
		{
			UE_LOG(LogTemp, Warning, TEXT("UGASO_AbilitySet::GiveToAbilitySystem: Invalid AttributeSet class in GrantedAttributes at index %d in AbilitySet %s"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(GASOASC->GetOwner(), AttributeSetToGrant.AttributeSet);
		GASOASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FGASO_AbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Warning, TEXT("UGASO_AbilitySet::GiveToAbilitySystem: Invalid GameplayAbility class in GrantedGameplayAbilities at index %d in AbilitySet %s"), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		// Set up the ability spec to grant the ability with the specified level and input tag.
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = GASOASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FGASO_AbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Warning, TEXT("UGASO_AbilitySet::GiveToAbilitySystem: Invalid GameplayEffect class in GrantedGameplayEffects at index %d in AbilitySet %s"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		// Set up the gameplay effect spec to grant the effect with the specified level.
		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = GASOASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, GASOASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}

	}
}

