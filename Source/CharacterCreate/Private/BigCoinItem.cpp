#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	PointValue = 50;
	ItemType = "BigCoin";
}

void ABigCoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}