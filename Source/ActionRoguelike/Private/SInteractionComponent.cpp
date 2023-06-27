// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponent::PrimaryInteract(UCameraComponent* camera)
{
	TArray<FHitResult> Hits;
	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector TraceStart = camera->GetComponentLocation();
	FRotator TraceRotation = camera->GetComponentRotation();
	FVector TraceEnd = TraceStart + (TraceRotation.Vector() * 500);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionShape Shape;
	float Radius = 50.f;
	Shape.SetSphere(Radius);
	
	GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = FColor::Red;
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
				if (HitActor->Implements<USGameplayInterface>())
				{
					APawn* MyPawn = Cast<APawn>(MyOwner);
					ISGameplayInterface::Execute_Interact(HitActor, MyPawn);

					DebugColor = FColor::Green;
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, DebugColor, false, 1.0f);
					break;
				}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, DebugColor, false, 1.0f, 0, 1.0f);
}
