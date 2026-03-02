// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBaseComponent.h"
#include <Camera/CameraComponent.h>

// Sets default values for this component's properties
UPickupBaseComponent::UPickupBaseComponent()
{
}


// Called when the game starts
void UPickupBaseComponent::BeginPlay()
{
	Super::BeginPlay();


	if (!bIsEnabled) return;

	SpawnPosition = Owner->GetActorLocation();
	
	OwnerMesh->SetMobility(EComponentMobility::Movable);
	OwnerMesh->SetSimulatePhysics(true);
	OwnerMesh->SetLinearDamping(1.0f);
	OwnerMesh->SetAngularDamping(1.0f);
}


// Called every frame
void UPickupBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPickupBaseComponent::Interact_Implementation(AActor* Interactor)
{
	if (Holder) { Throw(); return; }


	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) { Reset(); return; }

	Holder = Cast<APlayerController>(Pawn->GetController());
	if (!Holder) { Reset(); return; }

	UCameraComponent* Camera = Pawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) { Reset(); return; }

	Pickup(Camera);
}

bool UPickupBaseComponent::CanInteract_Implementation(AActor* Interactor) const
{
	if (!Interactor || !bIsEnabled) return false;

	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) return false;

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (!PlayerController) return false;

	return true;
}


void UPickupBaseComponent::Pickup(UCameraComponent* InteractorCamera)
{
	OwnerMesh->SetSimulatePhysics(false);
	OwnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Owner->AttachToComponent(InteractorCamera, FAttachmentTransformRules::KeepRelativeTransform);
	Owner->SetActorRelativeLocation(HoldOffset);

	Highlight_Implementation(false);
}

void UPickupBaseComponent::Reset()
{
	Holder = nullptr;

	Highlight_Implementation(false);
	
	Owner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OwnerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerMesh->SetSimulatePhysics(true);
}

void UPickupBaseComponent::Respawn()
{
	Owner->SetActorLocation(SpawnPosition);
}

void UPickupBaseComponent::Throw()
{
	if (Holder != nullptr)
	{
		FVector Direction = Holder->PlayerCameraManager->GetActorForwardVector();// (Owner->GetActorLocation() - Holder->PlayerCameraManager->GetCameraLocation()).GetSafeNormal();
		FVector Impulse = Direction * ThrowForce;

		Reset();

		OwnerMesh->AddImpulse(Impulse, NAME_None, true);
	}
}