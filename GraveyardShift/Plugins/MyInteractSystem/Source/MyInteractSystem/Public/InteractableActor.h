// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MyInteractableInterface.h"

#include "InteractableActor.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup = (Interact))
class MYINTERACTSYSTEM_API AInteractableActor : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupHighlight();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Highlight_Implementation(bool bValue);


protected:
	UPROPERTY()
	UMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEnabled = true;
};
