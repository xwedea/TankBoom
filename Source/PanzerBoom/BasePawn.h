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

private:
		
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UParticleSystem * DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USoundBase * DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

};
