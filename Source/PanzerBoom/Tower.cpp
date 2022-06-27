// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"


ATower::ATower() {
	UE_LOG(LogTemp, Warning, TEXT("ATower();"));
}

void  ATower::BeginPlay() {
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// DrawDebugSphere(
	// 	GetWorld(),
	// 	GetActorLocation(),
	// 	Range,
	// 	50,
	// 	FColor::Purple
	// );	


	if (TankInRange()) {
		RotateTurret(Tank->GetActorLocation());
	}

}

void ATower::CheckFireCondition() {
	if (bCanAttack && TankInRange()) {
		Fire();
	}
}


bool ATower::TankInRange() {
	if (Tank) {
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		
		if (Distance <= Range  && Tank->bAlive) {
			return true;
			// RotateTurret(Tank->GetActorLocation());
		}
	}
	return false;
}

void ATower::HandleDestruction() {
	Super::HandleDestruction();

	Destroy();
}








