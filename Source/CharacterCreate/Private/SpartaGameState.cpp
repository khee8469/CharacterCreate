#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaCharacter.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	CurrentLevelIndex = 0;
	MaxLevel = 3;


	CurrentWaveIndex = 0;
	MaxWave = 3;
	WaveDuration = 3.0f;
	ItemToSpawnPerWave = { 20,30,40 };

	//ItemToSpawn = 20;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	LevelInit();
}

void ASpartaGameState::ReLevel()
{
	WaveActorDestroy();

	LevelInit();
}

void ASpartaGameState::LevelInit()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		for (int32 i = 0; i < ItemToSpawnPerWave[CurrentWaveIndex]; i++)
		{
			if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem())
			{
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
				WaveActors.Add(SpawnedActor);
			}
		}
	}

	if (CurrentWaveIndex == 0)
	{
		FString str = FString::Printf(TEXT("%d"), CurrentWaveIndex);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, str);
	}
	else if (CurrentWaveIndex == 1)
	{
		FString str = FString::Printf(TEXT("%d"), CurrentWaveIndex);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, str);
		EnableWave2();
	}
	else if (CurrentWaveIndex == 2)
	{
		FString str = FString::Printf(TEXT("%d"), CurrentWaveIndex);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, str);
		EnableWave3();
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnWaveTimeUp,
		WaveDuration,
		false
	);
}

void ASpartaGameState::WaveActorDestroy()
{
	for (int i = 0; i < WaveActors.Num(); i++)
	{
		WaveActors[i]->Destroy();
	}

	WaveActors.Empty();
}

void ASpartaGameState::OnWaveTimeUp()
{
	CurrentWaveIndex++;
	if (CurrentWaveIndex >= MaxWave)
	{
		EndLevel();
	}
	else
	{
		ReLevel();
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (0 < SpawnedCoinCount && SpawnedCoinCount <= CollectedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (MaxLevel <= CurrentLevelIndex)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerContrller = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerContrller))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), SpartaGameInstance->TotalScore )));
						}
					}
				}

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d - %d"), CurrentLevelIndex+1, CurrentWaveIndex+1)));
				}

				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HP"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(MyPlayerController->GetPawn());
					if (SpartaCharacter)
					{
						HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %.0f / %.0f"), SpartaCharacter->Health, SpartaCharacter->MaxHealth)));
					}
				}
			}
		}
	}
}

void ASpartaGameState::EnableWave2()
{

}

void ASpartaGameState::EnableWave3()
{

}
