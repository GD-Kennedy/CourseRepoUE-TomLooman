// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "Math/UnitConversion.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName); 
	RootComponent = MeshComp;

    ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);
	
	ForceComp->ImpulseStrength = 1000.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->Radius = 500.0f;
}


// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);	
}


// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	if (OtherComp)
	{
		USAttributeComponent* AttributeComp =
			Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-50.0f);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

