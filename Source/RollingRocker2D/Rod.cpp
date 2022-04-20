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


// Called when the game starts
void URod::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URod::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URod::MoveLeftEnd(float scalar, float deltaTime)
{
	float scalarValue = FMath::Abs(scalar);
	float clampedScalarValue = FMath::Clamp(scalarValue, 0, 1);
	float clampedScalar = FMath::Sign(scalar) * clampedScalarValue;

	m_LeftEndHeight += clampedScalar * deltaTime * m_EndMaxSpeed;
	m_LeftEndHeight = FMath::Clamp(m_LeftEndHeight, -m_LeftEndMovableRange * 0.5f, m_LeftEndMovableRange * 0.5f);

	OnLeftEndLocationChanged.Broadcast(GetLeftEndLocation());
}

void URod::MoveRightEnd(float scalar, float deltaTime)
{
	float scalarValue = FMath::Abs(scalar);
	float clampedScalarValue = FMath::Clamp(scalarValue, 0, 1);
	float clampedScalar = FMath::Sign(scalar) * clampedScalarValue;

	m_RightEndHeight += clampedScalar * deltaTime * m_EndMaxSpeed;
	m_RightEndHeight = FMath::Clamp(m_RightEndHeight, -m_RightEndMovableRange * 0.5f, m_RightEndMovableRange * 0.5f);

	OnRightEndLocationChanged.Broadcast(GetRightEndLocation());
}