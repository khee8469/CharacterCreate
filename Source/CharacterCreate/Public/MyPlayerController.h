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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;


	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void EndGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void MainMenuReturn();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void BlindImage();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ImageBlind();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input System")
	UInputMappingContext* InputMappingContext;
	
protected:
	virtual void BeginPlay() override;
};
