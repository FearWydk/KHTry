// Elemental Action RPG — The Resonance
// Maurice (Cipher) Player Attribute Set
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_AttributeSet.h"
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

	// Focus — elemental magic resource consumed by element injection into combos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData Focus;
	ATTRIBUTE_ACCESSORS(UYH_PlayerAttributeSet, Focus)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData MaxFocus;
	ATTRIBUTE_ACCESSORS(UYH_PlayerAttributeSet, MaxFocus)

	// AttackPower — scales outgoing damage GameplayEffects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UYH_PlayerAttributeSet, AttackPower)

protected:

	UFUNCTION()
	virtual void OnRep_Focus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxFocus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
};
