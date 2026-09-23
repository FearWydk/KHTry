// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemComponent.h"
#include "GASO_ASI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGASO_ASI : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class GASOLINE_API IGASO_ASI
{
	GENERATED_BODY()


public:

	//GetAbilitySystemComponent
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GASO|AbilitySystem")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
};
