// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "SCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	SphereComp->SetSphereRadius(30.0f);
	MovementComp->InitialSpeed = 4500;
	LifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DashDelay, this, &ASDashProjectile::TriggerTeleport, 0.2f);
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TriggerTeleport();
}

void ASDashProjectile::TriggerTeleport()
{
	//// MOJ ORYGINALNY KOD
	// FVector Position = SphereComp->GetComponentLocation();
	// APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// Player->TeleportTo(Position, Player->GetActorRotation());

	APawn* Player = GetInstigator();
	Player->TeleportTo(GetActorLocation(), Player->GetActorRotation());
	Explode_Implementation();
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
