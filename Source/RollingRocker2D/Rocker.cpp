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

	if (!m_IsFreeMoveMode)
	{
		// Do acceleration calculation
		auto gravityDirection = FVector::DownVector;
		auto projectedAccelerationVector = gravityDirection.ProjectOnTo(m_Rod->GetRodVector());
		float accelerationDirection = FMath::Sign(projectedAccelerationVector.X);

		if (accelerationDirection != 0)
		{
			// Apply acceleration if the rod is not flat
			m_CurrentVelocity += accelerationDirection * projectedAccelerationVector.Length() * m_OnRodGravity * DeltaTime;
		}

		// Move location
		m_LocationOnRod += m_CurrentVelocity * DeltaTime;
	}

	// Update the actual world location based on location on rod
	FVector worldLocation = m_Rod->GetRodVector() * m_LocationOnRod;
}


void URocker::FreeMove(float moveDirection, float deltaTime)
{
	if (!m_Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Rocker's m_Rod is not valid."));
		return;
	}

	// Update location on rod
	float moveDirectionSign = FMath::Sign(moveDirection);
	m_LocationOnRod += moveDirectionSign * m_FreeMoveSpeed * deltaTime;
}
