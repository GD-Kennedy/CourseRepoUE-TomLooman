// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActionSystem/SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#define NAMEOF(name) #name

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.15f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		
		if (CastingVFX)
		{
			UGameplayStatics::SpawnEmitterAttached(CastingVFX, Character->GetMesh(), HandSocketName,
				FVector::Zero(), FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget);	
		}

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, NAMEOF(AttackDelay_Elapsed), Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void USAction_ProjectileAttack::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	ASCharacter* Character = Cast<ASCharacter>(InstigatorCharacter);
	if (ensure(ProjectileClass))
	{
		FVector TraceStart = Character->GetPawnViewLocation();
		FRotator TraceRotation = Character->GetPawnViewRotation();
		FVector TraceEnd = TraceStart + (TraceRotation.Vector() * 25000);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(5);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		
		FVector HandLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);

		FRotator ShootRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ShootRotation, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
