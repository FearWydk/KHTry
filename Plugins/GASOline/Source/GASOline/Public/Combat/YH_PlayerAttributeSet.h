// Elemental Action RPG — The Resonance
// Maurice (Cipher) Player Attribute Set
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "YH_PlayerAttributeSet.generated.h"

/**
 * Attribute set for Maurice and all player-controlled Ciphers.
 * Inherits Health and MaxHealth from UGASO_AttributeSet.
 * Adds Focus (elemental magic resource) and AttackPower.
 */
UCLASS()
class GASOLINE_API UYH_PlayerAttributeSet : public UGASO_AttributeSet
{
	GENERATED_BODY()

public:

	UYH_PlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Called automatically by GAS after any GameplayEffect modifies an attribute. Mirrors UYH_EnemyAttributeSet.
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Combat")
	bool bDeathStarted = false;
	
	
	// Focus — elemental magic resource consumed by element injection into combos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Vitals")
	FGameplayAttributeData Focus;
	ATTRIBUTE_ACCESSORS(UYH_PlayerAttributeSet, Focus)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Vitals")
	FGameplayAttributeData MaxFocus;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, MaxFocus)

	// AttackPower — scales outgoing damage GameplayEffects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, AttackPower)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, Strength)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData Element;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, Element)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData ElementPower;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, ElementPower)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, Armor)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, MovementSpeedMultiplier)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UYH_PlayerAttributeSet, MovementSpeed)
	
	
	
protected:

	UFUNCTION()
	virtual void OnRep_Focus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxFocus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
};
