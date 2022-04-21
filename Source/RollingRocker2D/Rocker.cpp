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

	float rodAreaWidth = m_Rod->GetMovableAreaWidth();
	float rodEndMovableRange = m_Rod->GetEndMovableRange();
	m_OnRodGravity = GetEvaluatedGravityFromTimeToReachMaxSpeed(rodAreaWidth, rodEndMovableRange);

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
		m_CurrentVelocity = m_FreeMoveDirectionBuffer * m_FreeMoveSpeed;
		if (!FMath::IsNearlyEqual(m_FreeMoveDirectionBuffer, 0))
		{
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
			float acceleration = accelerationDirection * projectedAccelerationVector.Length() * m_OnRodGravity * DeltaTime;
			float newVelocity = m_CurrentVelocity + acceleration;
			float newSpeed = FMath::Abs(newVelocity);
			if (newSpeed > m_ConstrainedMaxSpeed)
			{
				// New velocity value exceeds the max speed, clamp it
				float accelerationAdjustment = newSpeed - m_ConstrainedMaxSpeed;
				acceleration -= accelerationAdjustment * FMath::Sign(newVelocity);
			}

			m_CurrentVelocity += acceleration;
		}
	}

	// Move location
	locationOnRodDelta = m_CurrentVelocity * DeltaTime;

	bool isSqrtMaxExtentCalculated = false;
	float maxProjectedExtentOnRod = 0; // This value is only valid if isSqrtMaxExtentCalculated is true


	if (FMath::Abs(locationOnRodDelta) > 0)
	{
		// Move
		float actualAppliedDelta = InstantMoveClamp(locationOnRodDelta);
		RotateWithLocationDelta(actualAppliedDelta);
		if (actualAppliedDelta != locationOnRodDelta)
		{
			// Hit the wall / limit, reset velocity!
			m_CurrentVelocity = 0;
		}
	}
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

void URocker::RotateWithLocationDelta(float locationDelta)
{
	// Update rotation based on locationDelta
	float radianAngle = -locationDelta / (GetCollisionRadius());
	FVector SafeAxis = FVector::LeftVector.GetSafeNormal(); // Make sure axis is unit length
	FRotator rotation = FQuat(SafeAxis, radianAngle).Rotator();
	AddLocalRotation(rotation);
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
	float rockerRadius = GetCollisionRadius();
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
	SnapWorldLocationToRod();

	return locationDelta;
}

void URocker::SnapWorldLocationToRod()
{
	FVector normalizedRodVector = m_Rod->GetRodVector().GetSafeNormal();
	FVector rodCenterToWorldLocation = normalizedRodVector * m_LocationOnRod;
	FVector rodCenter = m_Rod->GetRodCenterLocation();
	FVector rockerWorldLocation = rodCenter + rodCenterToWorldLocation + FVector::UpVector * GetCollisionRadius();
	SetWorldLocation(rockerWorldLocation);
}

float URocker::GetEvaluatedGravityFromTimeToReachMaxSpeed(float rodMovableAreaWidth, float rodEndMovableRange)
{
	// Calculate the angle of the right triangle with the given two sides
	double tan = rodEndMovableRange / rodMovableAreaWidth;
	double maxRadianAngle = FMath::Atan(tan);

	float desiredAccelerationWithMaxAngle = m_ConstrainedMaxSpeed / m_TimeToReachConstrainedMaxSpeedWithMaxAngle;
	float gravity = desiredAccelerationWithMaxAngle / FMath::Sin(maxRadianAngle);

	return gravity;
}

#if WITH_EDITOR
void URocker::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	FName changedPropertyName = (propertyChangedEvent.Property != nullptr) ? propertyChangedEvent.Property->GetFName() : NAME_None;
	if (changedPropertyName == GET_MEMBER_NAME_CHECKED(URocker, m_TimeToReachConstrainedMaxSpeedWithMaxAngle))
	{
		// ... find a way to update gravity
	}
}
#endif