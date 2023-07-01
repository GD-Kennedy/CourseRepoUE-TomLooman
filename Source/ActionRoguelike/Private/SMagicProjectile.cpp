// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	DamageAmount = 10.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                       UPrimitiveComponent*, int32, bool,
                                       const FHitResult&)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(OtherActor))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

			Explode_Implementation();
		}
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}
