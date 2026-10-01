// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Generic AnimNotify that sends a GameplayEvent to the owning actor's ASC.
// GASoline Plugin | Combat Module
//
// Unreal's GameplayAbilities plugin does not ship a notify for this out of the box - most GAS
// projects build their own. Drop this on a montage timeline (e.g. where a sword swing should
// register a hit) and set EventTag to whatever a UAbilityTask_WaitGameplayEvent is listening for
// (e.g. YH.Combat.Notify.HitCheck).

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_YHGameplayEvent.generated.h"

UCLASS(meta = (DisplayName = "YH Gameplay Event"))
class KHTRY_API UAnimNotify_YHGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Tag sent to the owning actor's AbilitySystemComponent when this notify fires.
	UPROPERTY(EditAnywhere, Category = "GameplayEvent")
	FGameplayTag EventTag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
	// Shows the configured tag on the notify track instead of the generic class name.
	virtual FString GetNotifyName_Implementation() const override;
#endif
};
