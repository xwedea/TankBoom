// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "ChaserTowerBase.generated.h"

/**
 * 
 */
UCLASS()
class PANZERBOOM_API AChaserTowerBase : public ATower
{
	GENERATED_BODY()

public:
	AChaserTowerBase();
	virtual void Tick(float DeltaTime) override;
	
	void Chase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Movement")
		float ChaseRadius = 5000.f;

	UPROPERTY(EditAnywhere, Category="Movement")
		float ChaseDistance = 400.f;

	UPROPERTY(EditAnywhere, Category="Movement")
		bool InfiniteRange = true;
	
	UPROPERTY(EditAnywhere, Category="Movement")
		float ChaseSpeed = 100.f;



};
