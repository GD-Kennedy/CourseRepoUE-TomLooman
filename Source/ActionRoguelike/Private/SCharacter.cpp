// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "SAttributeComponent.h"
#include "SBasicProjectile.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChange);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ASCharacter::SpecialAttack);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::PlayerTeleport(FVector position, FRotator rotation)
{
	TeleportTo(position, rotation);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X forward (red)
	// Y right (green)
	// Z up (blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	StartProjectileAction(ProjectileClass, 0.2f);
}

void ASCharacter::SpecialAttack()
{
	PlayAnimMontage(AttackAnim);
	StartProjectileAction(ProjectileBPClass, 0.2f);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim, 2);
	StartProjectileAction(DashClass, 0.1f);
}

void ASCharacter::StartProjectileAction(TSubclassOf<AActor> projectile, float delay)
{
	UGameplayStatics::SpawnEmitterAttached(CastingVFX, GetMesh(), HandSocketName);	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_PrimaryAttack,
		[&, projectile] { SpawnProjectile(projectile); },
		delay,
		false);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> projectileType)
{
	if (ensure(projectileType))
	{
		FVector TraceStart = CameraComponent->GetComponentLocation();
		FRotator TraceRotation = CameraComponent->GetComponentRotation();
		FVector TraceEnd = TraceStart + (TraceRotation.Vector() * 25000);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(5);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		DrawDebugLine(GetWorld(), HandLocation, TraceEnd, FColor::Emerald, false, 1, 0, 2);

		FRotator ShootRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ShootRotation, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(projectileType, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract(CameraComponent);
	}
}

void ASCharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (NewHealth <= 0.0f)
	{
		if(Delta != 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
	else if (Delta < 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(FlashTimeToHitParam, GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedParam, HitFlashSpeedValue);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParam, HitFlashColorValue);
	}
}