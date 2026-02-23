// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBaseComponent.h"
#include <Camera/CameraComponent.h>
#include <Components/PostProcessComponent.h>

// Sets default values for this component's properties
UPickupBaseComponent::UPickupBaseComponent()
{
	Owner = GetOwner();
	if (!Owner) { bIsEnabled = false; }
	else
	{
		OwnerMesh = Owner->FindComponentByClass<UMeshComponent>();
		if (!OwnerMesh) { bIsEnabled = false; }
	}

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = bIsEnabled;

	SetActive(bIsEnabled);
}


// Called when the game starts
void UPickupBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsEnabled) return;

	SpawnPosition = Owner->GetActorLocation();



	UPostProcessComponent* PostProcessComp = NewObject<UPostProcessComponent>(this);
	PostProcessComp->SetupAttachment(Owner->GetRootComponent());
	PostProcessComp->RegisterComponent();

	PostProcessComp->BlendRadius = 3000.f;   // sphere radius in unreal units
	PostProcessComp->BlendWeight = 1.f;     // full effect inside radius

	UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
	PostProcessComp->Settings.AddBlendable(DynMat, 1.f);




	OwnerMesh->bRenderCustomDepth = true;
	OwnerMesh->SetCustomDepthStencilValue(1);
	Highlight(false);
	
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


void UPickupBaseComponent::Pickup(APlayerController* Player)
{
	Holder = Player;
	if (!Holder) { Reset(); return; }

	APawn* Pawn = Holder->GetPawn();
	if (!Pawn) { Reset(); return; }

	UCameraComponent* Camera = Pawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) { Reset(); return; }

	OwnerMesh->SetSimulatePhysics(false);
	OwnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Owner->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
	Owner->SetActorRelativeLocation(HoldOffset);

	Highlight(false);
}

void UPickupBaseComponent::Reset()
{
	Holder = nullptr;

	Highlight(false);
	
	Owner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OwnerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerMesh->SetSimulatePhysics(true);
}

void UPickupBaseComponent::Respawn()
{
	Owner->SetActorLocation(SpawnPosition);
}

void UPickupBaseComponent::Highlight(bool bValue)
{
	OwnerMesh->SetRenderCustomDepth(bValue);
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