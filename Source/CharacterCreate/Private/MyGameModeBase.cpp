#include "MyGameModeBase.h"
#include "SpartaCharacter.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
