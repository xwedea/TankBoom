// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundBase;

UCLASS()
class PANZERBOOM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * ProjectileMesh;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent * MovementComp;

	UFUNCTION()
		void OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	 	float Damage = 50;

	UPROPERTY(EditAnywhere)
		class UParticleSystem * HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent * TrailParticles;

	UPROPERTY(EditAnywhere, Category="Sounds")
		USoundBase * LaunchSound;
	UPROPERTY(EditAnywhere, Category="Sounds")
		USoundBase * HitSound;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

};
