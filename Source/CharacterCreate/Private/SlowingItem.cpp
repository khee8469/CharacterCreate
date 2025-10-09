#include "SlowingItem.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{
	//SlowPer = 50.0f;
	ItemType = "Slowing";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->DebuffSpeed();
		}

		DestroyItem();
	}
}
