// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class PANZERBOOM_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	ATower();

	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class ATank * Tank;
	float FireRate = 2;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",meta = (AllowPrivateAccess = "true"));
		float Range;
	
	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();
	bool TankInRange();

};
