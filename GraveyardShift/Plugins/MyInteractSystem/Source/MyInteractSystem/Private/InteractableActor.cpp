// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	

	Mesh = this->FindComponentByClass<UStaticMeshComponent>();

	if (!Mesh)
	{
		this->SetActorEnableCollision(false);
		this->SetActorTickEnabled(false);
		this->SetActorHiddenInGame(true);
	}
	else
		SetupHighlight();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractableActor::SetupHighlight()
{
	if (!Mesh) return;

	Mesh->bRenderCustomDepth = true;
	Mesh->SetCustomDepthStencilValue(1);
	Highlight_Implementation(false);
}

void AInteractableActor::Highlight_Implementation(bool bValue)
{
	Mesh->SetRenderCustomDepth(bValue);
}