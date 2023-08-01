// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USInteractionComponent();

protected:
	
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();
};
