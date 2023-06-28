#include "SBasicPickup.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"


ASBasicPickup::ASBasicPickup()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComp;
}

void ASBasicPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASBasicPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	RootComponent->SetVisibility(false);
	SetActorEnableCollision(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown,
		[=] {
		RootComponent->SetVisibility(true);
		SetActorEnableCollision(true);
	}, 10.0f, false);
}
