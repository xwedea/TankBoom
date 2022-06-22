// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Tower.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh");
	RootComponent = ProjectileMesh;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	MovementComp->InitialSpeed = 1200;
	MovementComp->MaxSpeed = 1500;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>("Trail Particles");
	TrailParticles->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult& HitResult) {

	AActor * MyOwner = GetOwner();
	if (!MyOwner) {
		Destroy();
		return;
	}
	
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageType = UDamageType::StaticClass();


	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		// UE_LOG(LogTemp, Warning, TEXT("applying damage..."));
		if (HitCameraShakeClass) 
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		

		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			MyOwnerInstigator,
			MyOwner,
			DamageType
		);

		// Hit Particle
		if (HitParticles) {
			UGameplayStatics::SpawnEmitterAtLocation(
				this->GetWorld(),
				HitParticles,
				GetTransform()
			);
		} else UE_LOG(LogTemp, Warning, TEXT("NO HIT PARTICLE"));
	}


	Destroy();


}
