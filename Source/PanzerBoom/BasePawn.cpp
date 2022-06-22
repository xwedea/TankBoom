// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret Mesh");
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("Spawn Point");
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePawn::HandleAllCountdowns() {
	// UE_LOG(LogTemp, Display, TEXT("BASEPAWN HANDLE ALL"));
	Countdown(FireRate, FireCountdown);
}

void ABasePawn::Fire() {
	if (FireCountdown != 0.f) return;
	FireCountdown = FireRate;

	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		Location,
		Rotation
	);
	Projectile->SetOwner(this);

}


void ABasePawn::RotateTurret(FVector TargetLoc) {

	FVector ToTarget = TargetLoc - GetActorLocation();
	FRotator Rotation = FRotator(
		0,
		ToTarget.Rotation().Yaw,
		0
	);
	TurretMesh->SetWorldRotation(Rotation);
}

void ABasePawn::HandleDestruction() {
	bAlive = false;
	if (DeathParticles)
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DeathParticles,
			GetTransform()
		);
	else UE_LOG(LogTemp, Warning, TEXT("%s: No DeathParticles"), *GetName());

	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			DeathSound,
			GetActorLocation()
		);
	else UE_LOG(LogTemp, Warning, TEXT("%s: No DeathSound"), *GetName());

	if (DeathCameraShakeClass)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
}

bool ABasePawn::IsCoolingDown(float &Rate, float &Countdown) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	// UE_LOG(LogTemp, Display, TEXT("Rate: %f, Countdown: %f"), Rate, Countdown);

	if (Countdown > 0.f) {
		Countdown -= DeltaTime;
		Countdown = (Countdown < 0.f) ? 0.f : Countdown;
		if (Countdown != 0.f) return true;
	}
	Countdown = Rate;
	return false;
}

void ABasePawn::Countdown(float &Rate, float &Countdown) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	if (Countdown > 0.f) {
		Countdown -= DeltaTime;
		Countdown = (Countdown < 0.f) ? 0.f : Countdown;
	}
	else if (Countdown < 0) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Countdown below zero!"), *GetActorNameOrLabel());
	}
	
}
