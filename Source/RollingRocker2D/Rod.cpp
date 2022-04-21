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
	
	bool leftEndMoved = false;
	bool rightEndMoved = false;

	FVector oldLeftLocation = GetLeftEndLocation();
	FVector oldRightLocation = GetRightEndLocation();

	if (!FMath::IsNearlyEqual(m_LeftEndMoveBuffer, 0))
	{
		m_LeftEndHeight += m_LeftEndMoveBuffer * DeltaTime * m_EndMaxSpeed;
		m_LeftEndHeight = FMath::Clamp(m_LeftEndHeight, -m_EndMovableRange * 0.5f, m_EndMovableRange * 0.5f);
		m_LeftEndMoveBuffer = 0;

		leftEndMoved = true;
		OnLeftEndLocationChanged.Broadcast(GetLeftEndLocation());
	}

	if (!FMath::IsNearlyEqual(m_RightEndMoveBuffer, 0))
	{
		m_RightEndHeight += m_RightEndMoveBuffer * DeltaTime * m_EndMaxSpeed;
		m_RightEndHeight = FMath::Clamp(m_RightEndHeight, -m_EndMovableRange * 0.5f, m_EndMovableRange * 0.5f);
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

		OnRodLocationChanged.Broadcast(data);
	}
}

void URod::MoveLeftEnd(float scalar)
{
	m_LeftEndMoveBuffer = FMath::Clamp(scalar, -1, 1);
}

void URod::MoveRightEnd(float scalar)
{
	m_RightEndMoveBuffer = FMath::Clamp(scalar, -1, 1);
}