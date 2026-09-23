// Elemental Action RPG — The Resonance
// Maurice (Cipher) Player Attribute Set
// GASoline Plugin | Combat Module


#include "Combat/YH_PlayerAttributeSet.h"

#include "Net/UnrealNetwork.h"

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
