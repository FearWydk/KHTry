// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// GASoline Plugin | Combat Module



#include "Combat/YH_EnemyAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GASO_CharacterBase.h"

UYH_EnemyAttributeSet::UYH_EnemyAttributeSet() : AttackPower(15.f)
{
	//Health = 0, Max Health = 100 set by UGASO_AttibuteSet base constructor
}

void UYH_EnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYH_EnemyAttributeSet, AttackPower);
}

void UYH_EnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Only handle Health changes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// The actual magnitude the effect applied.
		// Damage effects are negative, so flip the sign for a positive value that maches AC_Health's DecreaseHP function.

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
				if (GetHealth()<=0.f && !bDeathStarted)
				{
					bDeathStarted = true;

					FGameplayEventData OnDeathPayload;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						OwnerActor,
						FGameplayTag::RequestGameplayTag(FName("YH.Combat.Event.Death")),
						OnDeathPayload);
					UE_LOG(LogTemp, Warning, TEXT("Death Activated for %s"), *OwnerActor->GetName());

				}
			}
		}
	}
}


void UYH_EnemyAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_EnemyAttributeSet, AttackPower, OldValue);
}

