// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Generic AnimNotify that sends a GameplayEvent to the owning actor's ASC.
// GASoline Plugin | Combat Module

#include "Combat/AnimNotify_YHGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UAnimNotify_YHGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!EventTag.IsValid())
	{
		return;
	}

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
}

#if WITH_EDITOR
FString UAnimNotify_YHGameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag.IsValid() ? EventTag.ToString() : TEXT("YH Gameplay Event");
}
#endif
