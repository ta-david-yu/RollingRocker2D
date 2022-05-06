// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenManagerComponent.h"

void UTweener::InitForUse(FTweenUpdateDelegate const& updateDelegate)
{
	IsActive = true;
	IsPlaying = false;
	Timer = 0.0f;

	DelayTimer = 0.0f;
	Curve = nullptr;
	Duration = 1.0f;

	UpdateDelegate = FTweenUpdateDelegate{ updateDelegate };
	EndDelegate.Unbind();

	UpdateDynamicDelegate.Unbind();
	EndDynamicDelegate.Unbind();

	UpdateCallback = nullptr;
	EndCallback = nullptr;
}

void UTweener::InitForUse(FTweenUpdateDynamicDelegate const& updateDelegate)
{
	IsActive = true;
	IsPlaying = false;
	Timer = 0.0f;

	DelayTimer = 0.0f;
	Curve = nullptr;
	Duration = 1.0f;

	UpdateDelegate.Unbind();
	EndDelegate.Unbind();

	UpdateDynamicDelegate = FTweenUpdateDynamicDelegate{ updateDelegate };
	EndDynamicDelegate.Unbind();

	UpdateCallback = nullptr;
	EndCallback = nullptr;
}

void UTweener::InitForUse(TFunction<void(float)>&& updateCallback)
{
	IsActive = true;
	IsPlaying = false;
	Timer = 0.0f;

	DelayTimer = 0.0f;
	Curve = nullptr;
	Duration = 1.0f;

	UpdateDelegate.Unbind();
	EndDelegate.Unbind();

	UpdateDynamicDelegate.Unbind();
	EndDynamicDelegate.Unbind();

	UpdateCallback = MoveTemp(updateCallback);
	EndCallback = nullptr;
}

void UTweener::Update(float timeStep)
{
	if (DelayTimer > 0.0f)
	{
		DelayTimer -= timeStep;
		if (DelayTimer < 0.0f)
		{
			// TODO: do start callback (begin, update(0))
		}
	}
	else
	{
		Timer += timeStep;

		float tProgress = Timer / Duration;
		if (tProgress > 1.0f)
		{
			tProgress = 1.0f;
		}

		if (Curve->IsValidLowLevelFast())
		{
			float progress = Curve->GetFloatValue(tProgress);
			ExecuteUpdate(progress);
		}
		else
		{
			ExecuteUpdate(tProgress);
		}

		if (Timer > Duration)
		{
			ExecuteEnd();
			IsActive = false;
		}
	}
}

void UTweener::AbortWithEndCallback()
{
	if (IsActive)
	{
		ExecuteEnd();
	}

	IsActive = false;
}

void UTweener::AbortWithoutEndCallback()
{
	IsActive = false;
}

// Sets default values for this component's properties
UTweenManagerComponent::UTweenManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTweenManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTweenManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (auto& tweener : m_Tweeners)
	{
		if (!tweener->IsActive)
		{
			continue;
		}

		tweener->Update(DeltaTime);
	}
}

UTweener* UTweenManagerComponent::Tween(FTweenUpdateDelegate const& updateDelegate)
{
	UTweener* tweener = getInactiveTweener();
	tweener->InitForUse(updateDelegate);
	return tweener;
}

UTweener* UTweenManagerComponent::Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve)
{
	UTweener* tweener = getInactiveTweener();
	tweener->InitForUse(updateDelegate);
	tweener->Duration = duration;
	tweener->Curve = curve;
	return tweener;
}

UTweener* UTweenManagerComponent::Tween(TFunction<void(float)>&& updateCallback)
{
	UTweener* tweener = getInactiveTweener();
	tweener->InitForUse(MoveTemp(updateCallback));
	return tweener;
}

UTweener* UTweenManagerComponent::getInactiveTweener()
{
	for (auto& tweener : m_Tweeners)
	{
		if (!tweener->IsActive)
		{
			return tweener;
		}
	}

	UTweener* newTweener = NewObject<UTweener>();
	m_Tweeners.Add(newTweener);

	return newTweener;
}
