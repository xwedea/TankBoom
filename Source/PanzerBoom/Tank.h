// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class PANZERBOOM_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(float Value);
	void Turn(float Value);
	void HandleDestruction();
	

	void RotateTurret();

	APlayerController * GetTankController() const { return TankController; }
	AActor * GetLockedActor() const {return LockedActor;}
	void SetLockedActor(ABasePawn * Value) {LockedActor = Value;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
		class USpringArmComponent * SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
		class UCameraComponent * Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",meta = (AllowPrivateAccess = "true"));
		float Speed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",meta = (AllowPrivateAccess = "true"));
		float TurnRate = 100;
	UPROPERTY(EditAnywhere, Category = "Movement");
		float TurretTurnRate = 200;
	UPROPERTY(EditAnywhere, Category = "Combat");
		float AimRadius = 30;
	UPROPERTY(EditAnywhere, Category="Combat")
		float AimRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
		APlayerController * TankController;
	


	void Aim();
	bool bAiming = false;
	void AimLock();

	ABasePawn * AimedActor = nullptr;
	ABasePawn * LockedActor = nullptr;

	

	void DrawSphere(FVector Loc, const FColor &Color);

};
