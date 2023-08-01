// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.DebugDrawInteraction"),
	false,
	TEXT("Toogles drawing debug lines and spheres for interaction componment."),
	ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CollisionChannel = ECC_WorldDynamic;
	TraceRadius = 50.0f;
	TraceDistance = 700.0f;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
	{
		DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
	}
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	TArray<FHitResult> Hits;
	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector TraceStart;
	FRotator TraceRotation;
	if (ASCharacter* Character = Cast<ASCharacter>(MyOwner))
	{
		UCameraComponent* camera = Character->GetCamera();
		TraceStart = camera->GetComponentLocation();
		TraceRotation = camera->GetComponentRotation();
	}
	else
	{
		TraceStart = EyeLocation;
		TraceRotation = EyeRotation;
	}
	FVector TraceEnd = TraceStart + (TraceRotation.Vector() * TraceDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);
	FocusedActor = nullptr;
	
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	
	if (DefaultWidgetInstance)
	{
		if (FocusedActor)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
		else
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus Actor to interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}