#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CHARACTERCREATE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "Input System")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input System")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input System")
	UInputAction* JumpAction;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input System")
	UInputMappingContext* InputMappingContext;
	
protected:
	virtual void BeginPlay() override;
};
