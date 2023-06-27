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

