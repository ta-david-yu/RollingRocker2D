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

	// Evaluate gravity from the settings
	float rodAreaWidth = m_Rod->GetMovableAreaWidth();
	float rodEndMovableRange = m_Rod->GetEndMovableRange();
	m_OnRodGravity = GetEvaluatedGravityFromTimeToReachMaxSpeed(rodAreaWidth, rodEndMovableRange);

	// Evaluate deceleration from the settings
	m_OnRodDeceleration = m_ConstrainedMaxSpeed / m_TimeToCompletelyStopInConstrainedMode;
}


// Called every frame
void URocker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_MovementState == ERockerMovementState::External)
	{
		return;
	}

	if (!m_Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_Rod is not valid."));
		return;
	}

	FVector normalizedRodDirectionVector = m_Rod->GetRodVector().GetUnsafeNormal();
	FVector worldLocation = GetComponentLocation();
	float locationOnRodDelta = 0;

	// Do input handling
	if (m_MovementState == ERockerMovementState::Free)
	{
		m_CurrentVelocity = m_FreeMoveDirectionBuffer * m_FreeMoveSpeed;
		if (!FMath::IsNearlyEqual(m_FreeMoveDirectionBuffer, 0))
		{
			m_FreeMoveDirectionBuffer = 0;
		}
	}
	else if (m_MovementState == ERockerMovementState::Constrained)
	{
		// Do acceleration calculation
		float acceleration = calculateConstrainedModeAccelerationOnSlope(m_Rod->GetForwardVector(), normalizedRodDirectionVector);
		if (!FMath::IsNearlyZero(acceleration))
		{
			float accelerationThisFrame = acceleration * DeltaTime;
			float newVelocity = m_CurrentVelocity + accelerationThisFrame;

			// Acceleration capping
			float newSpeed = FMath::Abs(newVelocity);
			if (newSpeed > m_ConstrainedMaxSpeed)
			{
				// New velocity value exceeds the max speed, clamp it
				float accelerationAdjustment = newSpeed - m_ConstrainedMaxSpeed;
				accelerationThisFrame -= accelerationAdjustment * FMath::Sign(newVelocity);
			}

			m_CurrentVelocity += accelerationThisFrame;
		}
	}

	// Move location
	locationOnRodDelta = m_CurrentVelocity * DeltaTime;

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
	else
	{
		m_LastRotationDelta = FRotator::ZeroRotator;
	}
}

float URocker::calculateConstrainedModeAccelerationOnSlope(FVector groundDirection, FVector slopeDirection) const
{
	FVector rodForward = groundDirection.GetUnsafeNormal();
	double dot = FVector::DotProduct(rodForward, slopeDirection);
	double radianAngle = FMath::Acos(dot);
	double degreeAngle = FMath::RadiansToDegrees(radianAngle);
	if (degreeAngle > m_MinimumAccelerationAngle)
	{
		auto gravityDirection = FVector::DownVector;
		auto projectedAccelerationVector = gravityDirection.ProjectOnTo(slopeDirection);
		float accelerationDirection = FMath::Sign(projectedAccelerationVector.X);
		float acceleration = accelerationDirection * projectedAccelerationVector.Length() * m_OnRodGravity;
		return acceleration;
	}
	else
	{
		// Decelerate in the opposite direction
		if (FMath::IsNearlyZero(m_CurrentVelocity))
		{
			return 0;
		}
		else
		{
			return -FMath::Sign(m_CurrentVelocity) * m_OnRodDeceleration;
		}
	}
}

void URocker::SetRod(URod* rod)
{
	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.RemoveDynamic(this, &URocker::handleOnRodLocationChanged);
	}

	m_Rod = rod;

	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.AddDynamic(this, &URocker::handleOnRodLocationChanged);
	}
}

FRotator URocker::GetAngularVelocity(float locationDelta) const
{
	double radianAngle = -locationDelta / (GetCollisionRadius());
	FVector safeAxis = FVector::LeftVector.GetSafeNormal(); // Make sure axis is unit length
	FRotator rotation = FQuat(safeAxis, radianAngle).Rotator();
	return rotation;
}

void URocker::SetMovementState(ERockerMovementState state)
{
	ERockerMovementState prevState = m_MovementState;
	m_MovementState = state;
	if (m_MovementState == ERockerMovementState::Free)
	{
		m_CurrentVelocity = 0;
	}
}

void URocker::FreeMove(float moveDirection)
{
	m_FreeMoveDirectionBuffer = FMath::Clamp(moveDirection, -1, 1);
}

void URocker::RotateWithLocationDelta(float locationDelta)
{
	// Update rotation based on locationDelta
	m_LastRotationDelta = GetAngularVelocity(locationDelta);
	AddLocalRotation(m_LastRotationDelta);
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
	FVector rockerWorldLocation = m_Rod->GetWorldLocationFromPointOnRod(m_LocationOnRod) + FVector::UpVector * GetCollisionRadius();
	SetWorldLocation(rockerWorldLocation, true);
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

void URocker::handleOnRodLocationChanged(FRodLocationChangedEventData eventData)
{
	if (m_MovementState == ERockerMovementState::External)
	{
		return;
	}

	// Do Rocker location capping in case Rod length shrinks during its movement
	float rockerLocationOnRod = GetLocationOnRod();
	float rockerLocationSign = FMath::Sign(rockerLocationOnRod);
	float rockerEdgeLocationOnRod = rockerLocationOnRod + rockerLocationSign * GetCollisionRadius();
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