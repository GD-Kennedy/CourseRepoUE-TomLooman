// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameActionSystem/SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	DamageAmount = 10.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                       UPrimitiveComponent*, int32, bool,
                                       const FHitResult& HitResult)
{
	if (OtherActor && GetInstigator() != OtherActor)
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && !bIsBouncing && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			bIsBouncing = true;
			MoveComp->Velocity *= -1;

			if (APawn* NewInstigator = Cast<APawn>(OtherActor))
			{
				SetInstigator(NewInstigator);
			}
			return;
		}
		
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, HitResult))
		{
			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
			
			Explode_Implementation();
		};
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}
