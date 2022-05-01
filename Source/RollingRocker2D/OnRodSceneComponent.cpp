// Fill out your copyright notice in the Description page of Project Settings.


#include "OnRodSceneComponent.h"

// Sets default values for this component's properties
UOnRodSceneComponent::UOnRodSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOnRodSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOnRodSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOnRodSceneComponent::SetRod(URod* rod)
{
	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.RemoveDynamic(this, &UOnRodSceneComponent::handleOnRodLocationChanged);
	}

	m_Rod = rod;

	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.AddDynamic(this, &UOnRodSceneComponent::handleOnRodLocationChanged);
	}
}

FRotator UOnRodSceneComponent::GetAngularVelocity(float locationDelta) const
{
	double radianAngle = -locationDelta / m_Radius;
	FVector safeAxis = FVector::LeftVector.GetSafeNormal(); // Make sure axis is unit length
	FRotator rotation = FQuat(safeAxis, radianAngle).Rotator();
	return rotation;
}


float UOnRodSceneComponent::InstantMoveClamp(float locationDelta)
{
	if (!m_Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_Rod is not valid."));
		return 0;
	}

	float newLocationOnRod = m_LocationOnRod + locationDelta;
	float radius = m_Radius;
	float rodLengthExtent = m_Rod->GetRodVector().Length() * 0.5f - radius;
	float newLocationOnRodValue = FMath::Abs(newLocationOnRod);
	if (newLocationOnRodValue > rodLengthExtent)
	{
		float exceededAmount = newLocationOnRodValue - rodLengthExtent;
		float exceededAountAdjustment = -FMath::Sign(newLocationOnRod) * exceededAmount;
		locationDelta += exceededAountAdjustment;
	}

	m_LocationOnRod += locationDelta;

	// Update the actual world location based on location on rod
	SnapWorldLocationToRod();

	return locationDelta;
}

void UOnRodSceneComponent::RotateWithLocationDelta(float locationDelta)
{
	// Update rotation based on locationDelta
	m_LastRotationDelta = GetAngularVelocity(locationDelta);
	if (m_IsTransformAppliedOnActor)
	{
		GetOwner()->AddActorLocalRotation(m_LastRotationDelta);
	}
	else
	{
		AddLocalRotation(m_LastRotationDelta);
	}
}

void UOnRodSceneComponent::SnapWorldLocationToRod()
{
	FVector rockerWorldLocation = m_Rod->GetWorldLocationFromPointOnRod(m_LocationOnRod) + FVector::UpVector * m_Radius;

	if (m_IsTransformAppliedOnActor)
	{
		GetOwner()->SetActorLocation(rockerWorldLocation, true);
	}
	else
	{
		SetWorldLocation(rockerWorldLocation, true);
	}
}

void UOnRodSceneComponent::handleOnRodLocationChanged(FRodLocationChangedEventData eventData)
{
	// Do Rocker location capping in case Rod length shrinks during its movement
	float rockerLocationOnRod = GetLocationOnRod();
	float rockerLocationSign = FMath::Sign(rockerLocationOnRod);
	float rockerEdgeLocationOnRod = rockerLocationOnRod + rockerLocationSign * m_Radius;
	float sqrRockerEdgeLocationOnRod = rockerEdgeLocationOnRod * rockerEdgeLocationOnRod;

	float sqrRodLength = m_Rod->GetRodVector().SquaredLength();
	float sqrRodHalfLength = sqrRodLength / 4;

	if (sqrRockerEdgeLocationOnRod > sqrRodHalfLength)
	{
		// Rocker location is outside the rod length, do adjustment move
		float rodHalfLength = FMath::Sqrt(sqrRodLength) * 0.5f;
		float exceededAmount = FMath::Abs(rockerEdgeLocationOnRod) - rodHalfLength;
		float adjustmentDelta = -exceededAmount * rockerLocationSign;

		float actualAppliedDelta = InstantMoveClamp(adjustmentDelta);
		RotateWithLocationDelta(actualAppliedDelta);
	}
	else
	{
		SnapWorldLocationToRod();
	}
}