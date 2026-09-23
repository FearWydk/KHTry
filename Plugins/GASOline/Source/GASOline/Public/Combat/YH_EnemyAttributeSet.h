// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "GASO_AttributeSet.h"
#include "YH_EnemyAttributeSet.generated.h"

/**
 * Attribute set for all Wrath enemies (Light Wrath, Dark Wrath).
 * Inherits Health and MaxHealth from UGASO_AttributeSet.
 * Adds AttackPower for enemy damage output.
 */
UCLASS()
class GASOLINE_API UYH_EnemyAttributeSet : public UGASO_AttributeSet
{
	GENERATED_BODY()

public:

	UYH_EnemyAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data);

	// AttackPower — scales damage applied to Maurice on enemy attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UYH_EnemyAttributeSet, AttackPower)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	bool bDeathStarted = false;

protected:

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
};
