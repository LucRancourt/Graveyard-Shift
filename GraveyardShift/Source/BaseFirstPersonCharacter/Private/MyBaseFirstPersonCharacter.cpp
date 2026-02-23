// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseFirstPersonCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PickupBaseComponent.h"


// Sets default values
AMyBaseFirstPersonCharacter::AMyBaseFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // Create CameraComponent
    FPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FPCameraComponent != nullptr);

    // Attach it to the CapsuleComponent
    FPCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
    FPCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));// ? Also where did BaseEyeHeight come from?

    FPCameraComponent->bUsePawnControlRotation = true;

    SetupMeshes();
}

// Called when the game starts or when spawned
void AMyBaseFirstPersonCharacter::BeginPlay()
{
    check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("colors"));


    Super::BeginPlay();


    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called every frame
void AMyBaseFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




    FVector Start = FPCameraComponent->GetComponentLocation();
    FVector Forward = FPCameraComponent->GetForwardVector();
    FVector End = Start + Forward * 1000.0f;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

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
        UPickupBaseComponent* Pickup = HitActor->FindComponentByClass<UPickupBaseComponent>();

        if (PickupAhead != nullptr)
        {
            PickupAhead->Highlight(false);
            PickupAhead = nullptr;
        }

        if (Pickup)
        {
            PickupAhead = Pickup;
            PickupAhead->Highlight(true);
        }
    }
    else
    {
        if (PickupAhead != nullptr)
            PickupAhead->Highlight(false);

        PickupAhead = nullptr;
    }
}

// Called to bind functionality to input
void AMyBaseFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


    // Bind Input Actions
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyBaseFirstPersonCharacter::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyBaseFirstPersonCharacter::Look);
        }

        if (JumpAction)
        {
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyBaseFirstPersonCharacter::StartJump);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyBaseFirstPersonCharacter::StopJump);
        }

        if (InteractAction)
        {
            EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyBaseFirstPersonCharacter::Interact);
        }
    }
}


void AMyBaseFirstPersonCharacter::SetupMeshes()
{
    // Create MeshComponent
    FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    check(FPMesh != nullptr);

    FPMesh->SetOnlyOwnerSee(true); // FirstPersonMesh
    FPMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    USkeletalMeshComponent* TPMesh = GetMesh(); // ThirdPersonMesh
    TPMesh->SetOwnerNoSee(true);
    TPMesh->CastShadow = true; // Ensures shadow is enabled
    TPMesh->bCastHiddenShadow = true;
    TPMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

    // Attack it to the CameraComponent
    FPMesh->SetupAttachment(FPCameraComponent);
}


void AMyBaseFirstPersonCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Get Rotation for Direction (YawRot)
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

        // Get Directions
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add Movement based on Input
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AMyBaseFirstPersonCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
    }
}

void AMyBaseFirstPersonCharacter::StartJump() { bPressedJump = true; }
void AMyBaseFirstPersonCharacter::StopJump() { bPressedJump = false; }

void AMyBaseFirstPersonCharacter::Jump()
{
    if (Controller == nullptr) return;

    Super::Jump();
}

void AMyBaseFirstPersonCharacter::Interact()
{
    if (Controller == nullptr) return;

    if (HeldItem != nullptr)
    {
        HeldItem->Throw();
        HeldItem = nullptr;
        return;
    }

    if (PickupAhead != nullptr)
    {
        HeldItem = PickupAhead;
        HeldItem->Pickup(Cast<APlayerController>(Controller));
    }
}