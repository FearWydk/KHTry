// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActiveGameplayEffectHandle.h"
#include "GASO_AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayAbilitySpec.h" // for FGameplayAbilitySpecHandle
#include "GameplayEffectTypes.h" // for FActiveGameplayEffectHandle
#include "GASO_AbilitySet.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UGASO_ASC;
class UGameplayAbility;
class UObject;

/**
 * Data used by the ability set to grant abilities and gameplay effects to the ASC.
 */
USTRUCT(BlueprintType)
struct FGASO_AbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;

	// Ability Level
	UPROPERTY(EditDefaultsOnly, Category = "GASO")
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

};
/*
* Data used by the ability set to apply gameplay effects to the ASC.
*/
USTRUCT(BlueprintType)
struct FGASO_AbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly, Category = "GASO")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	// Level of the gameplay effect to apply. This is used to calculate the final magnitude of the gameplay effect.
	UPROPERTY(EditDefaultsOnly, Category = "GASO")
	float EffectLevel = 1.f;
};

/*
* Data used by the ability set to grant attribute sets to the ASC.
*/
USTRUCT(BlueprintType)
struct FGASO_AbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Attribute set to grant.
	UPROPERTY(EditDefaultsOnly, Category = "GASO")
	TSubclassOf<UAttributeSet> AttributeSet;
};
/**
* Data used to store handles to what has been granted by the ability set.
*/
USTRUCT(BlueprintType)
struct FGASO_AbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& SpecHandle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& EffectHandle);
	void AddAttributeSet(UAttributeSet* Set);
	void TakeFromAbilitySystem(UGASO_ASC* GASOASC);

protected:

	//Handles to the granted abilities, gameplay effects, and attribute sets. These are stored so they can be removed when the ability set is removed from the ASC.

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<UAttributeSet*> GrantedAttributeSets;
};

/**
* Non mutable data asset used to grant gameplay abilites and gameplay effects to an ASC.
*/
UCLASS()
class GASOLINE_API UGASO_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGASO_AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Grants the abilities, gameplay effects, and attribute sets in this ability set to the ASC.
	void GiveToAbilitySystem(UGASO_ASC* GASOASC, FGASO_AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "GASO", meta = (TitleProperty = Ability))
	TArray<FGASO_AbilitySet_GameplayAbility> GrantedGameplayAbilities;
	// Gameplay effects to apply when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "GASO", meta = (TitleProperty = GameplayEffect))
	TArray<FGASO_AbilitySet_GameplayEffect> GrantedGameplayEffects;
	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "GASO", meta = (TitleProperty = AttributeSet))
	TArray<FGASO_AbilitySet_AttributeSet> GrantedAttributes;
};
