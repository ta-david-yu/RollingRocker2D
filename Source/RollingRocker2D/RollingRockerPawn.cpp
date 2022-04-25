// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingRockerPawn.h"

// Sets default values
ARollingRockerPawn::ARollingRockerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARollingRockerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("RollingRockerPawn's Rod is not valid."));
		return;
	}

	if (!Rocker->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("RollingRockerPawn's Rocker is not valid."));
		return;
	}

	// Register Events
	Rod->OnRodLocationChanged.AddDynamic(this, &ARollingRockerPawn::handleOnRodLocationChanged);
}

// Called to bind functionality to input
void ARollingRockerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARollingRockerPawn::FallDownTo_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fall Down ~");
}

void ARollingRockerPawn::handleOnRodLocationChanged(FRodLocationChangedEventData eventData)
{
	// Update rod appearance (location & rotation)
	FVector centerLocation = (eventData.NewLeftLocation + eventData.NewRightLocation) * 0.5f;
	FVector safeAxis = FVector::LeftVector.GetSafeNormal();		// Make sure axis is unit length
	FRotator rotation = FQuat(safeAxis, Rod->GetSignedAngle()).Rotator();

	RodAppearance->SetWorldLocationAndRotationNoPhysics(centerLocation, rotation);

	// Do move forward logic
	float leftExceededAmount = eventData.LeftExceededHeightAmount;
	float rightExceededAmount = eventData.RightExceededHeightAmount;

	if (leftExceededAmount <= 0 && rightExceededAmount <= 0)
	{
		// No exceeding happens, early out.
		return;
	}

	FMoveForwardEventData data{ };
	data.MoveAmount = leftExceededAmount > rightExceededAmount ? leftExceededAmount : rightExceededAmount;
	OnMoveForward.Broadcast(data);
}