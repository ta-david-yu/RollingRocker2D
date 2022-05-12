// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScrollerSpeedController.h"

// Sets default values for this component's properties
ULevelScrollerSpeedController::ULevelScrollerSpeedController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelScrollerSpeedController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULevelScrollerSpeedController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float speedMultiplier = m_Scroller->GetAutoScrollSpeedMultiplier();
	if (m_IsScrolling)
	{
		if (speedMultiplier >= 1.0f)
		{
			// It's already in full speed
			return;
		}

		speedMultiplier += DeltaTime * (1.0f / m_TimeToChangeState);
		if (speedMultiplier > 1.0f)
		{
			speedMultiplier = 1.0f;
		}
		m_Scroller->SetAutoScrollSpeedMultiplier(speedMultiplier);
	}
	else
	{
		if (speedMultiplier <= 0.0f)
		{
			// It has already stopped
			return;
		}

		speedMultiplier -= DeltaTime * (1.0f / m_TimeToChangeState);
		if (speedMultiplier < 0.0f)
		{
			speedMultiplier = 0.0f;
		}
		m_Scroller->SetAutoScrollSpeedMultiplier(speedMultiplier);
	}
}


void ULevelScrollerSpeedController::SlowDown(float timeToFullySlowDown)
{
	m_IsScrolling = false;
	m_TimeToChangeState = timeToFullySlowDown;
}

void ULevelScrollerSpeedController::SpeedUp(float timeToFullySpeedUp)
{
	m_IsScrolling = true;
	m_TimeToChangeState = timeToFullySpeedUp;
}

