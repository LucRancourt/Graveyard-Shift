// Fill out your copyright notice in the Description page of Project Settings.


#include "InteracterComponent.h"

// Sets default values for this component's properties
UInteracterComponent::UInteracterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}


// Called when the game starts
void UInteracterComponent::BeginPlay()
{
	Super::BeginPlay();


    Owner = GetOwner();
    if (!Owner) { bIsEnabled = false; }
    else
    {
        OwnerCamera = Owner->FindComponentByClass<UCameraComponent>();
        if (!OwnerCamera) { bIsEnabled = false; }
    }

    if (!bIsEnabled)
    {
        PrimaryComponentTick.bCanEverTick = false;
        SetActive(false);
    }
}


// Called every frame
void UInteracterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


    if (ActiveInteractable && bIsInUse) return;

    FVector Start = OwnerCamera->GetComponentLocation();
    FVector Forward = OwnerCamera->GetForwardVector();
    FVector End = Start + Forward * TraceDistance;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();

        if (ActiveInteractable)
            IMyInteractableInterface::Execute_Highlight(ActiveInteractable.GetObject(), false);
        
        if (HitActor->GetClass()->ImplementsInterface(UMyInteractableInterface::StaticClass()))
        {
            ActiveInteractable.SetObject(HitActor);
            ActiveInteractable.SetInterface(Cast<IMyInteractableInterface>(HitActor));
        }
        else
        {
            TArray<UActorComponent*> Components = HitActor->GetComponents().Array();
            for (UActorComponent* Comp : Components)
            {
                if (Comp->GetClass()->ImplementsInterface(UMyInteractableInterface::StaticClass()))
                {
                    IMyInteractableInterface* Interface = Cast<IMyInteractableInterface>(Comp);
                    if (Interface)
                    {
                        ActiveInteractable.SetObject(Comp);
                        ActiveInteractable.SetInterface(Interface);
                        break;
                    }
                }
                else
                {
                    ActiveInteractable = nullptr;
                }
            }
        }

        if (ActiveInteractable)
            IMyInteractableInterface::Execute_Highlight(ActiveInteractable.GetObject(), true);
    }
    else
    {
        if (ActiveInteractable)
            IMyInteractableInterface::Execute_Highlight(ActiveInteractable.GetObject(), false);

        ActiveInteractable = nullptr;
    }
}

void UInteracterComponent::TryInteract()
{
    if (ActiveInteractable)
    {
        UObject* Object = ActiveInteractable.GetObject();

        if (Owner)
            if (IMyInteractableInterface::Execute_CanInteract(Object, Owner))
                IMyInteractableInterface::Execute_Interact(Object, Owner);

        bIsInUse = !bIsInUse;
    }
}