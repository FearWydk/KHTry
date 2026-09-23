#pragma once

#include "CoreMinimal.h"
#include "GASO_GAT.generated.h"

UENUM(BlueprintType)
enum class EGASO_AbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UENUM(BlueprintType)
enum class EGASO_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Ability1,
	Ability2,
	Ability3,
	Ability4,
	Ability5,
	Ability6,
	Ability7,
	Ability8,
	Ability9,
	Ability10
};