#include "ReverseControlItem.h"
#include "SpartaCharacter.h"

AReverseControlItem::AReverseControlItem()
{
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->ReverseControl();
		}

		DestroyItem();
	}
}
