


#include "Items/Fragments/ItemFragment_Usable.h"
#include "Items/Actions/ItemAction.h"

bool UItemFragment_Usable::Use(AActor* ItemOwner, UItemInstance* ItemInstance)
{
	bool bAnySucceed = false;

	for (const TObjectPtr<UItemAction>& Action : OnUseActions)
	{
		if (Action && Action->Execute(ItemOwner, ItemInstance))
		{
			bAnySucceed = true;
		}
	}
	return bAnySucceed;
}
