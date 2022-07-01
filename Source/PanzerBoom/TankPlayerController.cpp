// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() {
	Tank = Cast<ATank>(GetPawn());
}

void ATankPlayerController::SetPlayerEnabledState(bool bEnabled) {

	if (GetPawn()) {
		if (bEnabled) {
			GetPawn()->EnableInput(this);
		}
		else {
			GetPawn()->DisableInput(this);
		}
	} else UE_LOG(LogTemp, Warning, TEXT("%s: GetPawn() is nullptr"), *GetName());

	bShowMouseCursor = bEnabled;

}

FRotator ATankPlayerController::GetTSLocalRotation(float &controllerX, float &controllerY) {
	FVector vectorX = FVector(0, controllerX, 0);
	FVector vectorY = FVector(controllerY, 0, 0);
	return (vectorX + vectorY).Rotation();
}

FRotator ATankPlayerController::GetTSWorldRotation(float &controllerX, float &controllerY) {
	FVector vectorX = FVector(0, controllerX, 0);
	FVector vectorY = FVector(controllerY, 0, 0);
	FRotator LocalControllerRotation = (vectorX + vectorY).Rotation();

	FRotator SpringArmRotation = FRotator(
		0,
		Tank->GetSpringArmRotation().Yaw,
		0
	);

	FRotator FinalRotation = LocalControllerRotation+SpringArmRotation;

	return FinalRotation;
}