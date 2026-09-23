// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"


#include "GASO_AbilitySet.h"
#include "GASO_CharacterBase.generated.h"

class UGASO_ASC;
class UGASO_AttributeSet;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FGASOHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FGASOStaminaChanged, float, NewStamina, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FGASOManaChanged, float, NewMana, float, MaxMana);

UCLASS()
class GASOLINE_API AGASO_CharacterBase : public ACharacter, public IAbilitySystemInterface  
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASO_CharacterBase();

	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UGASO_ASC* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UGASO_AttributeSet* AttributeSet;

	//IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	EGameplayEffectReplicationMode GASO_ReplicationMode = EGameplayEffectReplicationMode::Mixed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
	TObjectPtr<UGASO_AbilitySet> CombatAbilitySet;

	UPROPERTY()
	FGASO_AbilitySet_GrantedHandles GrantedHandles;

	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleManaChanged(const FOnAttributeChangeData& Data);



private:

	void InitializeAbilitySystem();

public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "GAS")
	float GetCurrentHealth() const { return AttributeSet ? AttributeSet->GetHealth() : 0.f; }

	UFUNCTION(BlueprintPure, Category = "GAS")
	float GetMaxHealthValue() const { return AttributeSet ? AttributeSet->GetMaxHealth() : 0.f; }

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FGASOHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FGASOHealthChanged OnHealthDepleted;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FGASOStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FGASOManaChanged OnManaChanged;

	UPROPERTY()
	bool bAttributesInitialized = false;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GAS")
	void OnGASDamageReceived(float DamageAmount);

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	float DefaultMaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	float DefaultMaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	float DefaultMaxMana = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

public:
	UPROPERTY(EditAnywhere, Category = "AI|Patrol", meta = (MakeEditWidget = true))
	TArray<FVector> PatrolPoints;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Patrol")
	TArray<FVector> WorldPatrolPoints;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Patrol")
	int32 CurrentPatrolIndex = 0;
};
