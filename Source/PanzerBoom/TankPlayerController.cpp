// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

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