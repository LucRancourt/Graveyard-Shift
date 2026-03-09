// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InteractableComponent.h"

#include "PickupBaseComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Pickup), meta=(BlueprintSpawnableComponent))
class MYBASEPICKUP_API UPickupBaseComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupBaseComponent();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Reset();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact_Implementation(AActor* Interactor);
	bool CanInteract_Implementation(AActor* Interactor) const;
	bool IsDoubleInteract_Implementation() const;


	UFUNCTION()
	void Pickup(UCameraComponent* PlayerCamera);

	UFUNCTION()
	void Respawn();

	UFUNCTION()
	void Throw();
	

private:
	UPROPERTY()
	APlayerController* Holder;

	UPROPERTY()
	FVector SpawnPosition;

	UPROPERTY()
	FVector InitialScale;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HoldOffset = FVector(60.0f, 30.0f, -20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleAdjustment = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ThrowForce = 1000.0f;
};
