

#pragma once

#include "CoreMinimal.h"
#include "Items/Fragments/ItemFragment.h"
#include "ItemFragment_Usable.generated.h"

class UItemAction;
/**
 * 
 */
UCLASS()
class KHTRY_API UItemFragment_Usable : public UItemFragment
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	bool Use(AActor* ItemOwner, UItemInstance* ItemInst);
	bool Use(AActor* ItemOwner, UItemAction* ItemAction);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Options")
	bool bConsumeOnUse = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UItemAction>> OnUseActions;
	
};
