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


void UInteracterComponent::TempDisable(float Duration)
{
    bIsAllowedToTick = false;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &UInteracterComponent::ReEnable,
        Duration,
        false
    );
}

void UInteracterComponent::ReEnable()
{
    bIsAllowedToTick = true;
}

// Called every frame
void UInteracterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (!bIsAllowedToTick) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


    if (!IsValid(OwnerCamera) || !IsValid(Owner)) return;
    if (IsValid(ActiveInteractable.GetObject()) && bIsInUse) return;

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


    if (IsValid(ActiveInteractable.GetObject()) && ActiveInteractable != nullptr)
    {
        UObject* Object = ActiveInteractable.GetObject();
        if (IsValid(Object) && Object->Implements<UMyInteractableInterface>())
        {
            IMyInteractableInterface::Execute_Highlight(Object, false);
        }
    }

    ActiveInteractable = nullptr;


    if (!bHit) return;

    AActor* HitActor = HitResult.GetActor();
    if (!IsValid(HitActor)) return;
    
    if (!IsValid(Owner)) return;


    if (HitActor->Implements<UMyInteractableInterface>())
    {
        if (IMyInteractableInterface::Execute_CanInteract(HitActor, Owner))
        {
            if (IsValid(HitActor) && HitActor->Implements<UMyInteractableInterface>())
            {
                IMyInteractableInterface::Execute_Highlight(HitActor, true);

                ActiveInteractable.SetObject(HitActor);
                ActiveInteractable.SetInterface(Cast<IMyInteractableInterface>(HitActor));

                return;
            }
        }
    }

    for (UActorComponent* Comp : HitActor->GetComponents())
    {
        if (!Comp) continue;
        if (!Comp->Implements<UMyInteractableInterface>()) continue;

        if (IMyInteractableInterface::Execute_CanInteract(Comp, Owner))
        {
            if (IsValid(Comp) && Comp->Implements<UMyInteractableInterface>())
            {
                IMyInteractableInterface::Execute_Highlight(Comp, true);

                ActiveInteractable.SetObject(Comp);
                ActiveInteractable.SetInterface(Cast<IMyInteractableInterface>(Comp));

                break;
            }
        }
    }
}

void UInteracterComponent::TryInteract()
{
    if (ActiveInteractable == nullptr) return;

    if (!IsValid(ActiveInteractable.GetObject()))
    {
        ActiveInteractable = nullptr;
        return;
    }

    UObject* Object = ActiveInteractable.GetObject();
    if (!IsValid(Object))
    {
        ActiveInteractable = nullptr;
        return;
    }

    if (IsValid(Owner))
    {
        if (IMyInteractableInterface::Execute_CanInteract(Object, Owner))
        {
            IMyInteractableInterface::Execute_Interact(Object, Owner);

            if (IMyInteractableInterface::Execute_IsDoubleInteract(Object))
                bIsInUse = !bIsInUse;
        }
    }
}