#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class CHARACTERCREATE_API ASpartaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASpartaGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	//float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	TArray<AActor*> FoundVolumes;
	TArray<AActor*> WaveActors;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;


	//Wave
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentWaveIndex;
	int32 MaxWave;
	float WaveDuration;
	TArray<int32> ItemToSpawnPerWave;  // 웨이브 마다 스폰 아이템 수
	//int32 ItemToSpawn;

	

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void OnGameOver();


	void StartLevel();
	void ReLevel();
	void LevelInit();
	void WaveActorDestroy();
	void OnWaveTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();

	void EnableWave2();
	void EnableWave3();
};
