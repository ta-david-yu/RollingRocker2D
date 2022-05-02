// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenComponent.h"

// Sets default values for this component's properties
UTweenComponent::UTweenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTweenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!m_TweenCurve->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("TweenCompoment's Curve is not valid. A linear curve from 0 to 1 will be used instead."));
	}
}


// Called every frame
void UTweenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!m_IsPlaying)
	{
		return;
	}


	if (m_Timer < m_Duration)
	{
		m_Timer += DeltaTime;
		float tProgress = FMath::Clamp(m_Timer / m_Duration, 0, 1);

		if (m_TweenCurve->IsValidLowLevelFast())
		{
			float progress = m_TweenCurve->GetFloatValue(tProgress);
			OnTweenUpdate.Broadcast(progress);
		}
		else
		{
			OnTweenUpdate.Broadcast(tProgress);
		}

		if (m_Timer >= m_Duration)
		{
			OnTweenEnd.Broadcast();

			m_IsPlaying = false;
		}
	}
}

UTweenComponent* UTweenComponent::Tween()
{
	play();
	return this;
}

void UTweenComponent::play()
{
	m_IsPlaying = true;
	m_Timer = 0.0f;
}
