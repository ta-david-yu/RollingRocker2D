// Fill out your copyright notice in the Description page of Project Settings.


#include "RodAnimationController.h"

// Sets default values for this component's properties
URodAnimationController::URodAnimationController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URodAnimationController::SetRod(URod* rod)
{
	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.RemoveDynamic(this, &URodAnimationController::handleOnRodLocationChanged);
	}

	m_Rod = rod;

	if (m_Rod->IsValidLowLevel())
	{
		m_Rod->OnRodLocationChanged.AddDynamic(this, &URodAnimationController::handleOnRodLocationChanged);
	}
}

void URodAnimationController::handleOnRodLocationChanged(FRodLocationChangedEventData eventData)
{
	// Update rod appearance (location & rotation)
	FVector centerLocation = (eventData.NewLeftLocation + eventData.NewRightLocation) * 0.5f;
	FVector safeAxis = FVector::LeftVector.GetSafeNormal();		// Make sure axis is unit length
	FRotator rotation = FQuat(safeAxis, m_Rod->GetSignedAngle()).Rotator();

	SetWorldLocationAndRotationNoPhysics(centerLocation, rotation);
}

