// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASO_ASC.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (GAS), meta = (BlueprintSpawnableComponent))
class GASOLINE_API UGASO_ASC : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GAS|Init")
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
};
