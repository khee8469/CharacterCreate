#include "MyGameModeBase.h"
#include "SpartaCharacter.h"
#include "MyPlayerController.h"
#include "SpartaGameState.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}
