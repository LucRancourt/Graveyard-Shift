// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupBaseComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYBASEPICKUP_API UPickupBaseComponent : public UActorComponent
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

	UFUNCTION()
	void Pickup(APlayerController* Player);

	UFUNCTION()
	void Respawn();

	UFUNCTION()
	void Highlight(bool bValue);

	UFUNCTION()
	void Throw();
	



private:
	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	UMeshComponent* OwnerMesh;

	UPROPERTY()
	APlayerController* Holder;

	UPROPERTY()
	FVector SpawnPosition;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* OutlineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor HighlightColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector HoldOffset = FVector(60.0f, 30.0f, -20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ThrowForce = 1000.0f;
};
