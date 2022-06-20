// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANZERBOOM_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	void ActorDied(AActor * DeadActor);
	void HandleGameStart();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool bWin);

private:
	class ATank * Tank; 
	class ATankPlayerController * PlayerController;

	float StartDelay = 1;

	int volatile TowerCount;
	int FindTowerCount();


};
