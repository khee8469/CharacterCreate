#include "BlindItem.h"
#include "SpartaCharacter.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

ABlindItem::ABlindItem()
{

}

void ABlindItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerCharacter->GetController());
			MyPlayerController->BlindImage();
		}

		DestroyItem();
	}
}
