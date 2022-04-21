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
	// Location
	FVector centerLocation = (leftEndLocation + rightEndLocation) * 0.5f;

	// Rotation
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
}