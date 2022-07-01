// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserTowerBase.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

AChaserTowerBase::AChaserTowerBase() {

}

void AChaserTowerBase::BeginPlay() {
	Super::BeginPlay();

}

void AChaserTowerBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Chase();

}

void AChaserTowerBase::Chase() {
	if (!Tank) {
		UE_LOG(LogTemp, Warning, TEXT("%s: No Tank Found!"), *GetActorNameOrLabel());
		return;
	}
	FVector TankLoc = Tank->GetActorLocation();
	FVector TowerLoc = GetActorLocation();
	FVector Destination = (TowerLoc - TankLoc).Rotation().Vector() * ChaseDistance + TankLoc;
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	float Distance = FVector::Dist2D(TankLoc, TowerLoc);
	if (!InfiniteRange && Distance > ChaseRadius) {
		return;
	}

	FVector NewLoc = FMath::VInterpConstantTo(
		TowerLoc,
		Destination,
		DeltaTime,
		ChaseSpeed
	);
	SetActorLocation(NewLoc, true);
	TurretMesh->SetWorldRotation((TankLoc-TowerLoc).Rotation());
}

