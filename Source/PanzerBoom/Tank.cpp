// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Tower.h"

ATank::ATank() {
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	TankController = Cast<APlayerController>(GetController());

}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
	PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
	PlayerInputComponent->BindAxis("TurretRight");
	PlayerInputComponent->BindAxis("TurretForward");
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
	PlayerInputComponent->BindAction("AimLock", IE_Pressed, this, &ATank::AimLock);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RotateTurret();
	Aim();

	if (LockedActor){
		UE_LOG(LogTemp, Display, TEXT("Locked Actor: %s"), *LockedActor->GetName());
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("NO Locked Actor"));
	}

	
	if (LockedActor && LockedActor->bAlive) {
		DrawSphere(LockedActor->GetActorLocation(), FColor::Red);
		
		SetSpringArmRotationYaw(GetTurretRotation().Yaw);
	}

	
}

void ATank::AimLock() {
	if (LockedActor) {
		HandleTargetUnlock();
		return;
	}

	if (AimedActor) {
		LockedActor = AimedActor;

		// FRotator 
		
		// FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
		// NewSpringArmRotation.Yaw = TurretMesh->GetComponentRotation().Yaw;
		// SpringArm->SetWorldRotation(NewSpringArmRotation);
	
		SetSpringArmRotationYaw(GetTurretRotation().Yaw);


		return;
	}

}

void ATank::HandleTargetUnlock() {
	LockedActor = nullptr;
	SetSpringArmRotationYaw(GetActorRotation().Yaw);
}


void ATank::Aim() {

	FHitResult HitResult;
	FVector EndLoc = ProjectileSpawnPoint->GetComponentLocation() + \
		ProjectileSpawnPoint->GetForwardVector() * AimRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AimRadius);
	FCollisionShape CollisionBox = FCollisionShape::MakeBox(FVector(10, 10, 1));
	bAiming = GetWorld()->SweepSingleByChannel(
		HitResult,
		ProjectileSpawnPoint->GetComponentLocation() + FVector(0, 0, 100),
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		sphere
	);

	if (!bAiming) {
		AimedActor = nullptr;
		return;
	}

	AActor * HitActor = HitResult.GetActor();
	AimedActor = Cast<ABasePawn>(HitResult.GetActor());
	if (AimedActor) {
		if (AimedActor->ActorHasTag("Enemy")) {
			DrawSphere(AimedActor->GetActorLocation(), FColor::Green);
		}

	}
	else {
		HandleTargetUnlock();
	}

}

void ATank::RotateTurret() {
	FRotator NewRotation;

	if (!LockedActor) {
		float controllerX = GetInputAxisValue(TEXT("TurretRight"));
		float controllerY = GetInputAxisValue("TurretForward");
		if (controllerX == 0 && controllerY == 0) return;

		FVector vectorX = FVector(0, controllerX, 0);
		FVector vectorY = FVector(controllerY, 0, 0);
		FRotator LocalControllerRotation = (vectorX + vectorY).Rotation();
		// UE_LOG(LogTemp, Warning, TEXT("Controller Local Rotation: %s"), *LocalControllerRotation.ToString());

		FRotator SpringArmRotation = FRotator(
			0,
			SpringArm->GetComponentRotation().Yaw,
			0
		);
		// UE_LOG(LogTemp, Warning, TEXT("Camera World Rotation: %s"), *CameraRotation.ToString());

		FRotator FinalRotation = LocalControllerRotation+SpringArmRotation;
		// UE_LOG(LogTemp, Warning, TEXT("Final Rotation: %s"), *CameraRotation.ToString());

		NewRotation = FMath::RInterpConstantTo(
			TurretMesh->GetComponentRotation(),
			FinalRotation,
			GetWorld()->GetDeltaSeconds(),
			TurretTurnRate
		);
	}
	else {
		FVector ToTarget = LockedActor->GetActorLocation() - GetActorLocation();
		NewRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);
	}


	TurretMesh->SetWorldRotation(NewRotation);
}






void ATank::Move(float Value) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	float ForwardOffset = Value * Speed * DeltaTime;

	FVector DeltaLocation(ForwardOffset, 0.0f, 0.0f);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	float TurnVal = Value * TurnRate * DeltaTime;

	FRotator TurnOffset(0, TurnVal, 0);
	AddActorLocalRotation(TurnOffset, true);

}

void ATank::HandleDestruction() {
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ATank::DrawSphere(FVector Loc, const FColor &Color) {
	DrawDebugSphere(	
		GetWorld(),
		Loc,
		50,
		30,
		Color
	);
}

void ATank::SetSpringArmRotationYaw(float Yaw) {
	FRotator SpringArmRot = SpringArm->GetComponentRotation();
	SpringArmRot.Yaw = Yaw;
	SpringArm->SetWorldRotation(SpringArmRot);
}
