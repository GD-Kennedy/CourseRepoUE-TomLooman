// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasicProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASBasicProjectile::ASBasicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetNotifyRigidBodyCollision(true);
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionObjectType(ECC_WorldStatic);
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);
	
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;

	SetReplicates(true);
}

void ASBasicProjectile::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		FVector location = GetActorLocation();
		FRotator rotation = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, location, rotation);
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, location, rotation);
		
		AudioComp->FadeOut(0.1f, 0.0f);
		
		if (CameraShakeComponent)
		{
			UGameplayStatics::PlayWorldCameraShake(this, CameraShakeComponent, GetActorLocation(), 100.0f, 500.0f);
		}
		
		Destroy();
	}
}

void ASBasicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode_Implementation();
}

void ASBasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBasicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	SphereComp->OnComponentHit.AddDynamic(this, &ASBasicProjectile::OnActorHit);
	
	AudioComp->SetSound(FlightSound);
	AudioComp->Play();
}
