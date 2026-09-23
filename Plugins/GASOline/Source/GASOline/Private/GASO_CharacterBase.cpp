// Fill out your copyright notice in the Description page of Project Settings.


#include "GASO_CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASO_ASC.h"
#include "GASO_AbilitySet.h"
#include "GASO_AttributeSet.h"


// Sets default values
AGASO_CharacterBase::AGASO_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGASO_ASC>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(GASO_ReplicationMode);
	

	//Set size of capsule component to match the character's size. This is important for collision and movement.
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

}

UAbilitySystemComponent* AGASO_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGASO_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (CombatAbilitySet && AbilitySystemComponent)
	{
		CombatAbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
	for (const UAttributeSet* Set : AbilitySystemComponent->GetSpawnedAttributes())
	{
		UE_LOG(LogTemp, Warning, TEXT("  Set: %s (outer: %s)"),
			*GetNameSafe(Set), *GetNameSafe(Set ? Set->GetOuter() : nullptr));
	}
	InitializeAbilitySystem();

	WorldPatrolPoints.Reset();
	for (const FVector& Local : PatrolPoints)
	{
		WorldPatrolPoints.Add(GetActorTransform().TransformPosition(Local));
	}
	
}

void AGASO_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void AGASO_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();	
}



void AGASO_CharacterBase::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue, GetMaxHealthValue());
}

void AGASO_CharacterBase::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged.Broadcast(Data.NewValue, AttributeSet ? AttributeSet->GetMaxStamina() : 0.f);
}

void AGASO_CharacterBase::HandleManaChanged(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue, AttributeSet ? AttributeSet->GetMaxMana() : 0.f);
}

void AGASO_CharacterBase::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent) return;
	
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		if (!AttributeSet)
		{
			AttributeSet = const_cast<UGASO_AttributeSet*>(
				AbilitySystemComponent->GetSet<UGASO_AttributeSet>());
		}
		if (!AttributeSet)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: no GASO_AttributeSet found - is one listed in the ability set's GrantedAttributes?"), *GetName());
			return;
		}

		if (bAttributesInitialized) return;
		bAttributesInitialized = true;
			

			AttributeSet->SetMaxHealth(DefaultMaxHealth);
			AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
			AttributeSet->SetMaxStamina(DefaultMaxStamina);
			AttributeSet->SetStamina(AttributeSet->GetMaxStamina());
			AttributeSet->SetMaxMana(DefaultMaxMana);
			AttributeSet->SetMana(AttributeSet->GetMaxMana());

			UE_LOG(LogTemp, Warning, TEXT("Seeded: H %.1f/%.1f  S %.1f/%.1f  M %.1f/%.1f"),
				AttributeSet->GetHealth(), AttributeSet->GetMaxHealth(),
				AttributeSet->GetStamina(), AttributeSet->GetMaxStamina(),
				AttributeSet->GetMana(), AttributeSet->GetMaxMana());

			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
				.AddUObject(this, &AGASO_CharacterBase::HandleHealthChanged);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute())
				.AddUObject(this, &AGASO_CharacterBase::HandleStaminaChanged);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
				.AddUObject(this, &AGASO_CharacterBase::HandleManaChanged);

			OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealthValue());
			OnStaminaChanged.Broadcast(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
			OnManaChanged.Broadcast(AttributeSet->GetMana(), AttributeSet->GetMaxMana());

}

// Called to bind functionality to input
void AGASO_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGASO_CharacterBase::OnGASDamageReceived_Implementation(float DamageAmount)
{
	//Override in BP
}

