// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Combo Component
// GASoline Plugin | Combat Module


#include "Combat/ActorComponent/YH_ComboManager.h"
#include "Combat/ActorComponent/YH_WeaponManager.h"
#include "Combat/YH_CombatStatics.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Combat/ActorComponent/YH_ComboManager.h"

// Sets default values for this component's properties
UYH_ComboManager::UYH_ComboManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}




// Called when the game starts
void UYH_ComboManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	WeaponComp = GetOwner() ? GetOwner()->FindComponentByClass<UYH_WeaponManager>() : nullptr;

	if (!OwnerASC)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[ComboManager] No AbilitySystemComponent on %s - combo cannot hear hit events."),
			GetOwner() ? *GetOwner()->GetName() : TEXT("null owner"));
		return;
	}

	// Listen for the attack abiltiy's hit result on the owner's ASC
	if (HitConfirmedTag.IsValid())
	{
		HitConfirmedHandle = OwnerASC->GenericGameplayEventCallbacks
			.FindOrAdd(HitConfirmedTag)
			.AddUObject(this, &UYH_ComboManager::HandleHitConfirmed);
	}
	
}

void UYH_ComboManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwnerASC)
	{
		if (FGameplayEventMulticastDelegate* D = OwnerASC->GenericGameplayEventCallbacks.Find(HitConfirmedTag))
		{
			D->Remove(HitConfirmedHandle);
		}
		if (FGameplayEventMulticastDelegate* D = OwnerASC->GenericGameplayEventCallbacks.Find(HitMissedtag))
		{
			D->Remove(HitMissedHandle);
		}

		
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ComboWindowTimer);
		World->GetTimerManager().ClearTimer(AttackTimeoutTimer);
	}

	Super::EndPlay(EndPlayReason);
}

void UYH_ComboManager::OnAttackInput()
{
	// A finisher is committed - ignore input until it ends and its tag clears.
	if (IsFinisherActive())
	{
		return;
	}

	// Mid-attack: buffer the follow-up instead of starting a second attack now.
	if (bAttackActive)
	{
		bInputBuffered = true;
		return;
	}

	// Idle - fresh chain, or a late press inside the combo window after a confirmed hit.
	StartAttack();
}

void UYH_ComboManager::HandleHitConfirmed(const FGameplayEventData* Payload)
{

	//Ignore stray events with no attack happening.
	if (!bAttackActive)
	{
		return;
	}

	bAttackActive = false;
	// Which Weapon landed each hit this chain - this is what the finisher recipe reads.
	struct FComboHistoryEntry ComboEntry;

	//Add this hit to the combo history, which the finisher recipe will read. The recipe focuses on weapon+element conversion.
	ComboEntry.Weapon = PendingAttackWeapon;
	ComboEntry.Element = PendingElement;
	ComboHistory.Add(ComboEntry);
	

	++ComboIndex;
	// Clear the entry struct for the next hit - avoids stale data if the next attack doesn't set both fields.
	OnComboCountChanged.Broadcast(ComboIndex);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AttackTimeoutTimer);
	}

	// Reached the finsher: fire it, then zero out. Finisher.Active blocks input meanwhile.
	if (ComboIndex >= MaxComboHits)
	{
		
		// Evaluate the recipe based on the combo history, and fire the finisher event with the result.
		FComboResult Result = UYH_CombatStatics::EvaluateCombo(ComboHistory);
		OnRequestFinisher.Broadcast(Result);
		ResetCombo();
		return;
	}

	// Chain continues ONLY because we made contact. Buffered press -> attack immediately.
	if (bInputBuffered)
	{
		StartAttack();
		return;
	}

	// No follow-up buffered yet - open the window for a late press to continue the chain.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(ComboWindowTimer, this, &UYH_ComboManager::OnComboWindowExpired, ComboWindow, false);
	}
}

void UYH_ComboManager::HandleHitMissed(const FGameplayEventData* Payload)
{
	if (!bAttackActive)
	{
		return;
	}
	// Whiff breaks the chain - positioning matters.
	ResetCombo();
}

void UYH_ComboManager::StartAttack()
{
	bAttackActive = true;
	bInputBuffered = false;
	PendingAttackWeapon = GetActiveWeapon();
	PendingElement = GetActiveElement();

	if (UWorld* World = GetWorld())
	{
		// A new attack supersedes any open combo window.
		World->GetTimerManager().ClearTimer(ComboWindowTimer);
		// Saftey backstop in case the ability never reports a result.
		World->GetTimerManager().SetTimer(AttackTimeoutTimer, this,
			&UYH_ComboManager::OnAttackTimedOut, AttackSafteyTimeout, false);
	}

	// BP: play the montage for (weapon, comboIndex) and activate the matching attack ability.
	OnRequestAttack.Broadcast(PendingAttackWeapon, ComboIndex);	
}

void UYH_ComboManager::ResetCombo()
{
	ComboIndex = 0;
	bAttackActive = false;
	bInputBuffered = false;
	PendingAttackWeapon = EYH_WeaponType::None;
	PendingElement = EYH_ElementType::None;
	ComboHistory.Reset();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ComboWindowTimer);
		World->GetTimerManager().ClearTimer(AttackTimeoutTimer);
	}
	OnComboCountChanged.Broadcast(0);
}

void UYH_ComboManager::OnComboWindowExpired()
{
	//No follow up arrived in time.
	ResetCombo();
}

void UYH_ComboManager::OnAttackTimedOut()
{
	// The ability never reported Confirmed or Missed - free the state so input isn\t soft locked.
	UE_LOG(LogTemp, Warning,
		TEXT("[ComboManager] Attack timed out with no Hit.Confirmed/Missed - resetting combo."));
	ResetCombo();
}

EYH_WeaponType UYH_ComboManager::GetActiveWeapon() const
{
	return WeaponComp ? WeaponComp->GetActiveWeaponType() : EYH_WeaponType::None;
}

EYH_ElementType UYH_ComboManager::GetActiveElement() const
{
	return WeaponComp ? WeaponComp->GetActiveElementType() : EYH_ElementType::None;
}

bool UYH_ComboManager::IsFinisherActive() const
{
	return OwnerASC
		&& FinisherActiveTag.IsValid()
		&& OwnerASC->HasMatchingGameplayTag(FinisherActiveTag);
}

