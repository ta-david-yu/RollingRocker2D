// Fill out your copyright notice in the Description page of Project Settings.


#include "Rod.h"

// Sets default values for this component's properties
URod::URod()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void URod::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	bool leftEndMoved = false;
	bool rightEndMoved = false;

	float leftExceededHeightAmount = 0;
	float rightExceededHeightAmount = 0;

	FVector oldLeftLocation = GetLeftEndLocation();
	FVector oldRightLocation = GetRightEndLocation();

	if (!FMath::IsNearlyEqual(m_LeftEndMoveBuffer, 0))
	{
		float newHeight = m_LeftEndHeight + m_LeftEndMoveBuffer * DeltaTime * m_EndMaxSpeed;
		float clampedNewHeight = FMath::Clamp(newHeight, -m_EndMovableRange * 0.5f, m_EndMovableRange * 0.5f);

		leftExceededHeightAmount = FMath::Sign(newHeight) * FMath::Max(FMath::Abs(newHeight) - FMath::Abs(clampedNewHeight), 0);

		m_LeftEndHeight = clampedNewHeight;
		m_LeftEndMoveBuffer = 0;

		leftEndMoved = true;

		OnLeftEndLocationChanged.Broadcast(GetLeftEndLocation());
	}

	if (!FMath::IsNearlyEqual(m_RightEndMoveBuffer, 0))
	{
		float newHeight = m_RightEndHeight + m_RightEndMoveBuffer * DeltaTime * m_EndMaxSpeed;
		float clampedNewHeight = FMath::Clamp(newHeight, -m_EndMovableRange * 0.5f, m_EndMovableRange * 0.5f);

		rightExceededHeightAmount = FMath::Sign(newHeight) * FMath::Max(FMath::Abs(newHeight) - FMath::Abs(clampedNewHeight), 0);

		m_RightEndHeight = clampedNewHeight;
		m_RightEndMoveBuffer = 0;

		rightEndMoved = true;
		OnRightEndLocationChanged.Broadcast(GetRightEndLocation());
	}

	if (leftEndMoved || rightEndMoved)
	{
		FRodLocationChangedEventData data { };
		data.OldLeftLocation = oldLeftLocation;
		data.OldRightLocation = oldRightLocation;
		data.NewLeftLocation = GetLeftEndLocation();
		data.NewRightLocation = GetRightEndLocation();
		data.LeftExceededHeightAmount = FMath::Abs(leftExceededHeightAmount) > 0 ? leftExceededHeightAmount : 0;
		data.RightExceededHeightAmount = FMath::Abs(rightExceededHeightAmount) > 0 ? rightExceededHeightAmount : 0;

		OnRodLocationChanged.Broadcast(data);
	}
}

double URod::GetAngle() const
{
	FVector rodForward = GetForwardVector().GetUnsafeNormal();
	FVector normalizedRodVector = GetRodVector().GetUnsafeNormal();
	double dot = FVector::DotProduct(rodForward, normalizedRodVector);
	double radianAngle = FMath::Acos(dot);
	return radianAngle;
}

double URod::GetSignedAngle() const
{
	FVector rodForward = GetForwardVector().GetUnsafeNormal();
	FVector normalizedRodVector = GetRodVector().GetUnsafeNormal();

	FVector cross = FVector::CrossProduct(rodForward, normalizedRodVector);
	FVector screenPlaneNormal = FVector::LeftVector;
	float angleSign = FVector::DotProduct(cross, screenPlaneNormal) > 0 ? 1 : -1;

	double dot = FVector::DotProduct(rodForward, normalizedRodVector);
	double radianAngle = FMath::Acos(dot);
	return angleSign * radianAngle;
}

void URod::MoveLeftEnd(float scalar)
{
	m_LeftEndMoveBuffer = FMath::Clamp(scalar, -1, 1);
}

void URod::MoveRightEnd(float scalar)
{
	m_RightEndMoveBuffer = FMath::Clamp(scalar, -1, 1);
}