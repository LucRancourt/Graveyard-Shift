// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include <Camera/CameraComponent.h>
#include <MyInteractableInterface.h>

#include "InteracterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYINTERACTSYSTEM_API UInteracterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteracterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void TryInteract();

		
private:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	UCameraComponent* OwnerCamera;

	UPROPERTY()
	TScriptInterface<IMyInteractableInterface> ActiveInteractable;

	bool bIsInUse = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceDistance = 1000.0f;
};
