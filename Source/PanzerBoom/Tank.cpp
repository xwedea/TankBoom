// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RotateTurret();	
	Aim();
	
}

void ATank::Aim() {
	FHitResult HitResult;
	FVector EndLoc = ProjectileSpawnPoint->GetComponentLocation() + \
		ProjectileSpawnPoint->GetForwardVector() * AimRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(20);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		ProjectileSpawnPoint->GetComponentLocation(),
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		sphere
	);

	AActor * HitActor = HitResult.GetActor();
	if (bHit) {
		if (HitResult.GetActor()->ActorHasTag("Enemy")) {
			DrawDebugSphere(
				GetWorld(),
				HitActor->GetActorLocation(),
				50,
				30,
				FColor::Purple
			);
		}
	}
}

void ATank::RotateTurret() {
	float controllerX = GetInputAxisValue(TEXT("TurretRight"));
	float controllerY = GetInputAxisValue("TurretForward");
	if (controllerX == 0 && controllerY == 0) return;

	FVector vectorX = FVector(0, controllerX, 0);
	FVector vectorY = FVector(controllerY, 0, 0);
	FRotator LocalControllerRotation = (vectorX + vectorY).Rotation();
	// UE_LOG(LogTemp, Warning, TEXT("Controller Local Rotation: %s"), *LocalControllerRotation.ToString());

	FRotator CameraRotation = FRotator(
		0,
		Camera->GetComponentRotation().Yaw,
		0
	);
	// UE_LOG(LogTemp, Warning, TEXT("Camera World Rotation: %s"), *CameraRotation.ToString());

	FRotator FinalRotation = LocalControllerRotation+CameraRotation;
	// UE_LOG(LogTemp, Warning, TEXT("Final Rotation: %s"), *CameraRotation.ToString());

	FRotator NewRotation = FMath::RInterpConstantTo(
		TurretMesh->GetComponentRotation(),
		FinalRotation,
		GetWorld()->GetDeltaSeconds(),
		TurretTurnRate
	);
	TurretMesh->SetWorldRotation(NewRotation);
}




void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
	PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
	PlayerInputComponent->BindAxis("TurretRight");
	PlayerInputComponent->BindAxis("TurretForward");

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

	bAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

