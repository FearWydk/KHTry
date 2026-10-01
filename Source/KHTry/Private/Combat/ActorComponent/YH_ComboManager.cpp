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

	if (HitMissedtag.IsValid())
	{
		HitMissedHandle = OwnerASC->GenericGameplayEventCallbacks
			.FindOrAdd(HitMissedtag)
			.AddUObject(this, &UYH_ComboManager::HandleHitMissed);
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

	// Which weapon landed this hit - this is what the finisher recipe reads.
	// Only a landed hit counts toward the recipe; a whiff carries no weapon/element into it.
	FComboHistoryEntry ComboEntry;
	ComboEntry.Weapon = PendingAttackWeapon;
	ComboEntry.Element = PendingElement;
	ComboHistory.Add(ComboEntry);

	AdvanceCombo();
}

void UYH_ComboManager::HandleHitMissed(const FGameplayEventData* Payload)
{
	if (!bAttackActive)
	{
		return;
	}

	// A whiff still advances the string - hack-n-slash combos keep going when you swing at
	// air, they just don't feed the finisher recipe. Positioning is rewarded via damage/finisher
	// quality elsewhere, not by punting the player back to combo index 0.
	AdvanceCombo();
}

void UYH_ComboManager::AdvanceCombo()
{
	bAttackActive = false;
	++ComboIndex;
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

	// Buffered press (pressed mid-swing) -> next attack fires on the next tick.
	// AdvanceCombo() is reached from inside the CURRENT attack's HitCheck AnimNotify
	// callback (still mid-montage), so calling StartAttack() here synchronously would ask
	// the AnimInstance to start a new montage while it's still inside a notify callback of
	// the one being replaced. That reentrancy into the anim system is what was making
	// combo hits intermittently get dropped when the player spammed the attack button.
	// Deferring by a single tick (imperceptible) gets us out of that callback first.
	if (bInputBuffered)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(this, &UYH_ComboManager::StartAttack);
		}
		else
		{
			StartAttack();
		}
		return;
	}

	// No follow-up buffered yet - open the window for a late press to continue the chain.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(ComboWindowTimer, this, &UYH_ComboManager::OnComboWindowExpired, ComboWindow, false);
	}
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

