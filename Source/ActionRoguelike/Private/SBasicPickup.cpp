#include "SBasicPickup.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"

ASBasicPickup::ASBasicPickup()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComp;

	SetReplicates(true);
}

void ASBasicPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
	if (!Player)
	{
		return;
	}

	RootComponent->SetVisibility(false);
	SetActorEnableCollision(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown,
		[=] {
		RootComponent->SetVisibility(true);
		SetActorEnableCollision(true);
	}, 10.0f, false);
}
