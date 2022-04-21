// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingRockerPawn.h"

// Sets default values
ARollingRockerPawn::ARollingRockerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	Rod->OnRodLocationChanged.AddDynamic(this, &ARollingRockerPawn::HandleOnRodLocationChanged);
}

// Called every frame
void ARollingRockerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARollingRockerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARollingRockerPawn::HandleOnRodLocationChanged(FVector leftEndLocation, FVector rightEndLocation)
{
	// Rod appearance location
	FVector centerLocation = (leftEndLocation + rightEndLocation) * 0.5f;

	// Rod appearance rotation
	FVector normalizedRodVector = (rightEndLocation - leftEndLocation).GetUnsafeNormal();
	FVector rodForward = Rod->GetForwardVector().GetUnsafeNormal();

	double dot = FVector::DotProduct(rodForward, normalizedRodVector);
	double radianAngle = FMath::Acos(dot);

	FVector cross = FVector::CrossProduct(rodForward, normalizedRodVector);
	FVector screenPlaneNormal = FVector::LeftVector;
	float angleSign = FVector::DotProduct(cross, screenPlaneNormal) > 0 ? 1 : -1;

	FVector SafeAxis = FVector::LeftVector.GetSafeNormal(); // Make sure axis is unit length
	FRotator rotation = FQuat(SafeAxis, angleSign * radianAngle).Rotator();

	RodAppearance->SetWorldLocationAndRotationNoPhysics(centerLocation, rotation);

	// Do Rocker location capping in case Rod length shrinks during its movement
	float rockerLocationOnRod = Rocker->GetLocationOnRod();
	float rockerLocationSign = FMath::Sign(rockerLocationOnRod);
	float rockerEdgeLocationOnRod = Rocker->GetLocationOnRod() + rockerLocationSign * Rocker->GetCollisionRadius();
	float sqrRockerEdgeLocationOnRod = rockerEdgeLocationOnRod * rockerEdgeLocationOnRod;

	float sqrRodLength = Rod->GetRodVector().SquaredLength();
	float sqrRodHalfLength = sqrRodLength / 4;

	if (sqrRockerEdgeLocationOnRod > sqrRodHalfLength)
	{
		// Rocker location is outside the rod length, do adjustment move
		float exceededAmount = rockerEdgeLocationOnRod - FMath::Sqrt(sqrRodLength) * 0.5f;
		float adjustmentDelta = -exceededAmount * rockerLocationSign;

		Rocker->InstantMoveClamp(adjustmentDelta);
	}
	else
	{
		Rocker->SnapWorldLocationToRod();
	}
}