// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocker.h"

// Sets default values for this component's properties
URocker::URocker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URocker::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URocker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_Rod is not valid."));
		return;
	}

	FVector normalizedRodDirectionVector = m_Rod->GetRodVector().GetUnsafeNormal();
	FVector worldLocation = GetComponentLocation();
	float locationOnRodDelta = 0;

	// Do playing input handling
	if (m_IsFreeMoveMode)
	{
		if (!FMath::IsNearlyEqual(m_FreeMoveDirectionBuffer, 0))
		{
			locationOnRodDelta = m_FreeMoveDirectionBuffer * m_FreeMoveSpeed * DeltaTime;
			m_FreeMoveDirectionBuffer = 0;
		}
	}
	else
	{
		// Do acceleration calculation
		auto gravityDirection = FVector::DownVector;
		auto projectedAccelerationVector = gravityDirection.ProjectOnTo(normalizedRodDirectionVector);
		float accelerationDirection = FMath::Sign(projectedAccelerationVector.X);

		if (accelerationDirection != 0)
		{
			// Apply acceleration if the rod is not flat
			m_CurrentVelocity += accelerationDirection * projectedAccelerationVector.Length() * m_OnRodGravity * DeltaTime;
		}

		// Move location
		locationOnRodDelta = m_CurrentVelocity * DeltaTime;
	}

	bool isSqrtMaxExtentCalculated = false;
	float maxProjectedExtentOnRod = 0; // This value is only valid if isSqrtMaxExtentCalculated is true


	if (FMath::Abs(locationOnRodDelta) > 0)
	{
		// Move
		float actualAppliedDelta = InstantMoveClamp(locationOnRodDelta);
		if (actualAppliedDelta != locationOnRodDelta)
		{
			// Hit the wall / limit, reset velocity!
			m_CurrentVelocity = 0;
		}
	}


	// We want to move this to Rod logic when ends are changed
	
	// If rocker is on an end of the rod and the rod end moves toward its origin,
	// the rod length in x axis would be shorter, which will limit rocker's location.
	// In that case, we want to push rocker inside a bit.
	/*
	float rodAreaExtent = m_Rod->GetMovableAreaWidth() * 0.5f;
	FVector rodAreaOriginToRightEndOrigin = m_Rod->GetForwardVector() * rodAreaExtent;
	float sqrMaxProjectedExtentOnRod = rodAreaOriginToRightEndOrigin.ProjectOnTo(normalizedRodDirectionVector).SquaredLength();
	float sqrLocationOnRod = m_LocationOnRod * m_LocationOnRod;
	if (sqrLocationOnRod > sqrMaxProjectedExtentOnRod)
	{


		// Location on rod magnitude has exceeded the current
		// ...
	}*/

	// Update the actual world location based on location on rod
	//FVector worldLocation = normalizedRodDirectionVector * m_LocationOnRod;
}

void URocker::ActivateFreeMoveMode() 
{ 
	m_IsFreeMoveMode = true;
	m_CurrentVelocity = 0;
}

void URocker::DeactivateFreeMoveMode() 
{ 
	m_IsFreeMoveMode = false;
	m_CurrentVelocity = 0;
}

void URocker::FreeMove(float moveDirection)
{
	m_FreeMoveDirectionBuffer = FMath::Clamp(moveDirection, -1, 1);
}

float URocker::InstantMoveClamp(float locationDelta)
{
	if (!m_Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_Rod is not valid."));
		return 0;
	}

	if (!m_SphereCollision->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_SphereCollision is not valid."));
		return 0;
	}

	float newLocationOnRod = m_LocationOnRod + locationDelta;
	float rockerRadius = m_SphereCollision->GetScaledSphereRadius();
	float rodLengthExtent = m_Rod->GetRodVector().Length() * 0.5f - rockerRadius;
	float newLocationOnRodValue = FMath::Abs(newLocationOnRod);
	if (newLocationOnRodValue > rodLengthExtent)
	{
		float exceededAmount = newLocationOnRodValue - rodLengthExtent;
		float exceededAountAdjustment = -FMath::Sign(newLocationOnRod) * exceededAmount;
		locationDelta += exceededAountAdjustment;
	}

	m_LocationOnRod += locationDelta;

	// Update the actual world location based on location on rod
	FVector normalizedRodVector = m_Rod->GetRodVector().GetSafeNormal();
	FVector rodCenterToWorldLocation = normalizedRodVector * m_LocationOnRod;
	FVector rodCenter = m_Rod->GetRodCenterLocation();
	FVector rockerWorldLocation = rodCenter + rodCenterToWorldLocation + FVector::UpVector * rockerRadius;

	SetWorldLocation(rockerWorldLocation);


	// Update rotation based on locationDelta
	float radianAngle = -locationDelta / (rockerRadius);
	FVector SafeAxis = FVector::LeftVector.GetSafeNormal(); // Make sure axis is unit length
	FRotator rotation = FQuat(SafeAxis, radianAngle).Rotator();

	AddLocalRotation(rotation);
	
	return locationDelta;
}
