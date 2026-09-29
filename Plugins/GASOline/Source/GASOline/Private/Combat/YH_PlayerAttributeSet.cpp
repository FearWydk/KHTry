// Elemental Action RPG — The Resonance
// Maurice (Cipher) Player Attribute Set
// GASoline Plugin | Combat Module


#include "Combat/YH_PlayerAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASO_CharacterBase.h"

UYH_PlayerAttributeSet::UYH_PlayerAttributeSet() :
	Focus(0.f),
	MaxFocus(100.f),
	AttackPower(10.f)
{
	// Health and MaxHealth initialized by UGASO_AttributeSet base constructor
}

void UYH_PlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYH_PlayerAttributeSet, Focus);
	DOREPLIFETIME(UYH_PlayerAttributeSet, MaxFocus);
	DOREPLIFETIME(UYH_PlayerAttributeSet, AttackPower);
}

void UYH_PlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Only handle Health changes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// The actual magnitude the effect applied.
		// Damage effects are negative, so flip the sign for a positive value that matches AC_Health's DecreaseHP function.
		float DeltaValue = Data.EvaluatedData.Magnitude;
		float DamageAmount = -DeltaValue;

		// Keep the GAS Health attribute clamped in valid range
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		// Only forward actual damage (ignore healing or zero deltas)
		if (DamageAmount > 0.f)
		{
			AActor* OwnerActor = GetOwningActor();
			if (AGASO_CharacterBase* OwnerCharacter =
				Cast<AGASO_CharacterBase>(OwnerActor))
			{
				OwnerCharacter->OnGASDamageReceived(DamageAmount);

				// Only react if the hit wasn't lethal
				if (GetHealth() > 0.f)
				{
					FGameplayEventData HitReactPayload;
					HitReactPayload.OptionalObject = OwnerCharacter->HitReactMontage;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						OwnerActor,
						FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.HitReact")),
						HitReactPayload);
				}
				if (GetHealth() <= 0.f && !bDeathStarted)
				{
					bDeathStarted = true;

					FGameplayEventData OnDeathPayload;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						OwnerActor,
						FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.Death")),
						OnDeathPayload);
				}
			}
		}
	}
}

void UYH_PlayerAttributeSet::OnRep_Focus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_PlayerAttributeSet, Focus, OldValue);
}

void UYH_PlayerAttributeSet::OnRep_MaxFocus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_PlayerAttributeSet, MaxFocus, OldValue);
}

void UYH_PlayerAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_PlayerAttributeSet, AttackPower, OldValue);
}
