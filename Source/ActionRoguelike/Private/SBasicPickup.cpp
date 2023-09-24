#include "SBasicPickup.h"

#include "SCharacter.h"
#include "Net/UnrealNetwork.h"

ASBasicPickup::ASBasicPickup()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComp;

	bReplicates = true;
}

void ASBasicPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
	if (!Player)
	{
		return;
	}

	TogglePowerUp(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown,
		[=] {
			TogglePowerUp(true);
	}, 10.0f, false);
}

FText ASBasicPickup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASBasicPickup::TogglePowerUp(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASBasicPickup::OnRep_IsActive()
{
	RootComponent->SetVisibility(bIsActive, true);
	SetActorEnableCollision(bIsActive);
}

void ASBasicPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASBasicPickup, bIsActive);
}
