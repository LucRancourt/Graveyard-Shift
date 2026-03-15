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


	SetupInteractableValues();
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UInteractableComponent::SetupHighlight()
{
	OwnerMesh->bRenderCustomDepth = true;
	OwnerMesh->SetCustomDepthStencilValue(CustomDepthStencilValue);
	Highlight_Implementation(false);
}

void UInteractableComponent::Highlight_Implementation(bool bValue)
{
	OwnerMesh->SetRenderCustomDepth(bValue);
}

bool UInteractableComponent::CanInteract_Implementation(AActor* Interactor) const
{
	if (!bIsEnabled) return false;

	return true;
}

void UInteractableComponent::SetIsEnabled(bool bEnabled)
{
	bIsEnabled = bEnabled;
	SetupInteractableValues();
}

void UInteractableComponent::SetupInteractableValues()
{
	if (!bIsEnabled)
	{
		Owner = nullptr;

		if (IsValid(OwnerMesh))
		{
			Highlight_Implementation(false);
		}

		OwnerMesh = nullptr;

		PrimaryComponentTick.bCanEverTick = bIsEnabled;
		SetActive(bIsEnabled);

		return;
	}

	Owner = GetOwner();
	if (!Owner) { bIsEnabled = false; }
	else
	{
		OwnerMesh = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (!OwnerMesh) { bIsEnabled = false; }
	}

	PrimaryComponentTick.bCanEverTick = bIsEnabled;
	SetActive(bIsEnabled);

	if (!bIsEnabled) return;

	SetupHighlight();
}