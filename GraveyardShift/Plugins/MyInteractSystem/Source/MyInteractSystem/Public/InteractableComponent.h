// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MyInteractableInterface.h"

#include "InteractableComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class MYINTERACTSYSTEM_API UInteractableComponent : public UActorComponent, public IMyInteractableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupHighlight();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Highlight_Implementation(bool bValue);


protected:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	UMeshComponent* OwnerMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEnabled = true;
};