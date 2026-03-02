// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();


	if (!bIsEnabled) return;

	Owner = GetOwner();
	if (!Owner) { bIsEnabled = false; }
	else
	{
		OwnerMesh = Owner->FindComponentByClass<UMeshComponent>();
		if (!OwnerMesh) { bIsEnabled = false; }
	}

	PrimaryComponentTick.bCanEverTick = bIsEnabled;

	SetActive(bIsEnabled);

	if (!bIsEnabled) return;

	SetupHighlight();
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UInteractableComponent::SetupHighlight()
{
	Owner = GetOwner();
	if (!Owner) return;

	OwnerMesh = Owner->FindComponentByClass<UMeshComponent>();
	if (!OwnerMesh) return;

	OwnerMesh->bRenderCustomDepth = true;
	OwnerMesh->SetCustomDepthStencilValue(1);
	Highlight_Implementation(false);
}

void UInteractableComponent::Highlight_Implementation(bool bValue)
{
	OwnerMesh->SetRenderCustomDepth(bValue);
}