// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PANZERBOOM_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	void SetPlayerEnabledState(bool bEnabled);

	FRotator GetTSWorldRotation(float &controllerX, float &controllerY);
	FRotator GetTSLocalRotation(float &controllerX, float &controllerY);

protected:
	virtual void BeginPlay();

private:
	class ATank * Tank;

};
