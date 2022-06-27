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
	FVector TankLoc = Tank->GetActorLocation();
	FVector TowerLoc = GetActorLocation();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	float Distance = FVector::Dist2D(TankLoc, TowerLoc);

	if (Distance > ChaseRadius) {
		return;
	}

	FVector NewLoc = FMath::VInterpConstantTo(
		TowerLoc,
		TankLoc,
		DeltaTime,
		ChaseSpeed
	);

	SetActorLocation(NewLoc, true);

	TurretMesh->SetWorldRotation((TankLoc-TowerLoc).Rotation());

}

