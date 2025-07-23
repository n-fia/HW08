
#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	ItemType = "BigCoin";
	PointValue = 100;
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
