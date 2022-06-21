// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "TankPlayerController.h"
#include "Engine/EngineTypes.h"


void AToonTanksGameMode::BeginPlay() {
	Super::BeginPlay();
	HandleGameStart();
	TowerCount = FindTowerCount();
}

void AToonTanksGameMode::HandleGameStart() {
	StartGame(); // BP Implementable
	
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (PlayerController) {
		PlayerController->SetPlayerEnabledState(false);
		FTimerHandle StartTimerHandle;
		FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&ATankPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(
			StartTimerHandle,
			StartTimerDelegate,
			StartDelay,
			false
		);

	}

}


void AToonTanksGameMode::ActorDied(AActor * DeadActor) {
	
	if (DeadActor == Tank) 
	{
		Tank->HandleDestruction();
		Tank->SetActorTickEnabled(false);
		
		if (PlayerController) {
			PlayerController->SetPlayerEnabledState(false);
		} 
		GameOver(false);
		
	} 
	else 
	{
		if (DeadActor == Tank->GetLockedActor()) {
			Tank->HandleTargetUnlock();
		}

		if (ATower * DeadTower = Cast<ATower>(DeadActor)) {
			DeadTower->HandleDestruction();
			TowerCount--;
			if (TowerCount == 0) GameOver(true);
		}
	}

}

int AToonTanksGameMode::FindTowerCount() {
	TArray<AActor *> Towers;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		ATower::StaticClass(),
		Towers
	);

	return Towers.Num();
}

