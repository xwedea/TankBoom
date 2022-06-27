// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class PANZERBOOM_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	void HandleDestruction();
	bool bAlive = true;
	UPROPERTY(EditAnywhere, Category = "Combat")
		bool bCanAttack = true;

	FVector GetTurretLocation() const;

	bool IsCoolingDown(float &Rate, float &Countdown);

	void Countdown(float &Rate, float &Countdown);

	float GetCapsuleRadius() const;

	virtual void HandleAllCountdowns();

	virtual void Tick(float DeltaTime) override;


protected:
	void RotateTurret(FVector ToTarget);
	
	void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent * CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent * BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent * TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent * ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	
private:

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UParticleSystem * DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USoundBase * DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;


	UPROPERTY(EditAnywhere, Category="Combat|Timer")
		float FireRate = 0.1f;
	float FireCountdown = 0.f;

};
