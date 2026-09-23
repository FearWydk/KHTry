

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemAction.generated.h"

class UItemInstance;
class AActor;
/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract, DefaultToInstanced, EditInlineNew)
class KHTRY_API UItemAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	bool Execute(AActor* ItemOwner, UItemInstance* ItemInst);
	virtual bool Execute_Implementation(AActor* ItemOwner, UItemInstance* ItemInst){ return false; }
};
