// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Combo Component — contact-gated buffered combo (Kingdom Hearts II feel)
// GASoline Plugin | Combat Module

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include <Delegates/DelegateCombinations.h>
#include "../YH_CombatTypes.h"
#include "Containers/Array.h"
#include "YH_ComboManager.generated.h"



class UAbilitySystemComponent;
class UYH_WeaponManager;
struct FGameplayEventData;

//BP plays the montage and activates the matching attack ability for (weapon, comboIndex).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestAttack, EYH_WeaponType, Weapon, int32, ComboIndex);


// BP plays the finisher montage and activates the finisher ability. WeaponHistory feeds the recipe.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestFinisher, FComboResult, ComboResult);

// HUD hook for a combo counter. ComboIndex feeds the display.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboCountChanged, int32, NewCount);


/**
 * Contact-gated, buffered combo controller.
 *
 * Owns the combo STATE and the GATE LOGIC only — it never plays montages.
 * Animation and ability activation stay in Blueprint via the OnRequest* delegates,
 * matching the project's "animation lives in BP" split. The attack ability reports
 * back through the ASC with YH.Combat.Hit.Confirmed (landed) or YH.Combat.Hit.Missed
 * (whiffed); this controller advances the chain on a confirm and resets it on a miss
 * or a window timeout. That is the KH2 gate: input is always buffered, but it only
 * ADVANCES on contact.
 *
 * Wiring:
 *   - Character attack input  -> OnAttackInput()
 *   - Bind OnRequestSwing     -> play (weapon, index) montage + TryActivateAbilitiesByTag
 *   - Bind OnRequestFinisher  -> play finisher montage + activate finisher ability
 *   - Finisher ability owns YH.Combat.Finisher.Active (ActivationOwnedTags); that tag
 *     blocks basic attacks (their ActivationBlockedTags) and weapon switches
 *     (WeaponComponent.BlockSwitchTags), so finishers are uninterruptible.
 */


UCLASS( ClassGroup=(YH), meta=(BlueprintSpawnableComponent) )
class GASOLINE_API UYH_ComboManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYH_ComboManager();

	/* Call from the character's attack input */
	UFUNCTION(BlueprintCallable, Category = "YH|Combat|Combo")
	void OnAttackInput();

	UFUNCTION(BlueprintPure, Category = "YH|Combat|Combo")
	int32 GetComboIndex() const { return ComboIndex; }

	// Bound in BP: play the montage and activate the attack ability for this (weapon, comboIndex)
	UPROPERTY(BlueprintAssignable, Category = "YH|Combat|Combo")
	FOnRequestAttack OnRequestAttack;

	// Bound in BP: Play the finisher and activate the finisher ability.
	UPROPERTY(BlueprintAssignable, Category = "YH|Combat|Combo")
	FOnRequestFinisher OnRequestFinisher;

	// Bound in BP: update a HUD combo counter.
	UPROPERTY(BlueprintAssignable, Category = "YH|Combat|Combo")
	FOnComboCountChanged OnComboCountChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// --- Tuning ---

	// Hits landed before a finisher fires (3 = three attacks then finisher).
	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	int32 MaxComboHits = 3;

	// Seconds after a confirmed hit during which the next input still continues the chain.
	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	float ComboWindow = 1.2f;
	
	// Backstop: if a attack never reports Confirmed or Misssed within this long, free the state so input can't soft-lock 
	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	float AttackSafteyTimeout = 2.0f;

	// --- Tags (assign these to your project tags in the editor) ---
	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	FGameplayTag HitConfirmedTag; // YH.Combat.Hit.Confirmed

	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	FGameplayTag HitMissedtag; // YH.Combat.Hit.Missed

	UPROPERTY(EditAnywhere, Category = "YH|Combat|Combo")
	FGameplayTag FinisherActiveTag; // YH.Combat.Finisher.Active
	

private:
	void HandleHitConfirmed(const FGameplayEventData* Payload);
	void HandleHitMissed(const FGameplayEventData* Payload);

	void StartAttack();
	void ResetCombo();
	void OnComboWindowExpired();
	void OnAttackTimedOut();

	EYH_WeaponType GetActiveWeapon() const;
	EYH_ElementType GetActiveElement() const;
	bool IsFinisherActive() const;

	int32 ComboIndex = 0;
	bool bAttackActive = false;
	bool bInputBuffered = false;

	// Weapon equipped when the current attack stated; recorded into history on a confirmed hit.
	EYH_WeaponType PendingAttackWeapon = EYH_WeaponType::None;
	EYH_ElementType PendingElement = EYH_ElementType::None;

	
	TArray <FComboHistoryEntry> ComboHistory; // For easy iteration in the recipe evaluator.


	FTimerHandle ComboWindowTimer;
	FTimerHandle AttackTimeoutTimer;

	UPROPERTY() TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;
	UPROPERTY() TObjectPtr <UYH_WeaponManager> WeaponComp = nullptr;

	FDelegateHandle HitConfirmedHandle;
	FDelegateHandle HitMissedHandle;
		
};
