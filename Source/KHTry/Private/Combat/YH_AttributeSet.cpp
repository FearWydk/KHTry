// Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Shared combat attribute set — base for every YH_* character (player and enemy alike).
// KHTry Game Module | Combat

#include "Combat/YH_AttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASO_CharacterBase.h"

UYH_AttributeSet::UYH_AttributeSet()
{
	InitStrength(10.f);
	InitMagic(10.f);
	InitDefense(5.f);
	InitAttackPower(10.f);
	InitMagicPower(10.f);
	InitSpeed(10.f);
	InitMovementSpeedMultiplier(1.f);
	InitCritical(5.f);
	InitCritMultiplier(1.5f);
	InitIncomingDamage(0.f);
	InitIncomingHealing(0.f);
}

void UYH_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYH_AttributeSet, Strength);
	DOREPLIFETIME(UYH_AttributeSet, Magic);
	DOREPLIFETIME(UYH_AttributeSet, Defense);
	DOREPLIFETIME(UYH_AttributeSet, AttackPower);
	DOREPLIFETIME(UYH_AttributeSet, MagicPower);
	DOREPLIFETIME(UYH_AttributeSet, Speed);
	DOREPLIFETIME(UYH_AttributeSet, MovementSpeedMultiplier);
	DOREPLIFETIME(UYH_AttributeSet, Critical);
	DOREPLIFETIME(UYH_AttributeSet, CritMultiplier);
	// IncomingDamage/IncomingHealing are meta attributes - deliberately not replicated.
}

void UYH_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& ChangedAttribute = Data.EvaluatedData.Attribute;

	if (ChangedAttribute == GetIncomingDamageAttribute())
	{
		// Consume the meta attribute immediately so it never carries a stale value into the
		// next hit, then turn it into a real Health change.
		const float DamageDone = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (DamageDone <= 0.f)
		{
			return;
		}

		SetHealth(FMath::Clamp(GetHealth() - DamageDone, 0.f, GetMaxHealth()));

		AGASO_CharacterBase* OwnerCharacter = Cast<AGASO_CharacterBase>(GetOwningActor());
		HandleDamageTaken(DamageDone, OwnerCharacter);
		return;
	}

	if (ChangedAttribute == GetIncomingHealingAttribute())
	{
		const float HealingDone = GetIncomingHealing();
		SetIncomingHealing(0.f);

		if (HealingDone <= 0.f)
		{
			return;
		}

		SetHealth(FMath::Clamp(GetHealth() + HealingDone, 0.f, GetMaxHealth()));
		return;
	}
}

void UYH_AttributeSet::HandleDamageTaken(float DamageAmount, AGASO_CharacterBase* OwnerCharacter)
{
	if (!OwnerCharacter)
	{
		return;
	}

	OwnerCharacter->OnGASDamageReceived(DamageAmount);

	// Only react if the hit wasn't lethal.
	if (GetHealth() > 0.f)
	{
		FGameplayEventData HitReactPayload;
		HitReactPayload.OptionalObject = OwnerCharacter->HitReactMontage;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OwnerCharacter,
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.HitReact")),
			HitReactPayload);
		return;
	}

	if (!bDeathStarted)
	{
		bDeathStarted = true;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OwnerCharacter,
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.Death")),
			FGameplayEventData());
	}
}

void UYH_AttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, Strength, OldValue);
}

void UYH_AttributeSet::OnRep_Magic(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, Magic, OldValue);
}

void UYH_AttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, Defense, OldValue);
}

void UYH_AttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, AttackPower, OldValue);
}

void UYH_AttributeSet::OnRep_MagicPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, MagicPower, OldValue);
}

void UYH_AttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, Speed, OldValue);
}

void UYH_AttributeSet::OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, MovementSpeedMultiplier, OldValue);
}

void UYH_AttributeSet::OnRep_Critical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, Critical, OldValue);
}

void UYH_AttributeSet::OnRep_CritMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_AttributeSet, CritMultiplier, OldValue);
}
