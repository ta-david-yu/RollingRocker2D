// Fill out your copyright notice in the Description page of Project Settings.


#include "DYTweenManager.h"


void UDYTweener::InitForUse(FTweenUpdateDelegate const& updateDelegate)
{
	IsActive = true;
	IsPlaying = false;
	Timer = 0.0f;

	DelayTimer = 0.0f;
	Curve = nullptr;
	Duration = 1.0f;

	UpdateDelegate = FTweenUpdateDelegate{updateDelegate};
	EndDelegate.Unbind();

	UpdateDynamicDelegate.Unbind();
	EndDynamicDelegate.Unbind();

	UpdateCallback = nullptr;
	EndCallback = nullptr;
}

void UDYTweener::InitForUse(FTweenUpdateDynamicDelegate const& updateDelegate)
{
	IsActive = true;
	IsPlaying = false;
	Timer = 0.0f;

	DelayTimer = 0.0f;
	Curve = nullptr;
	Duration = 1.0f;

	UpdateDelegate.Unbind();
	EndDelegate.Unbind();

	UpdateDynamicDelegate = FTweenUpdateDynamicDelegate{updateDelegate};
	EndDynamicDelegate.Unbind();

	UpdateCallback = nullptr;
	EndCallback = nullptr;
}

void UDYTweener::InitForUse(TFunction<void(float)>&& updateCallback)
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

void UDYTweener::Update(float timeStep)
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

void UDYTweener::AbortWithEndCallback()
{
	if (IsActive)
	{
		ExecuteEnd();
	}

	IsActive = false;
}

void UDYTweener::AbortWithoutEndCallback()
{
	IsActive = false;
}

void DYTweenManager::Update(float deltaTime)
{
	for (auto& tweener : m_Tweeners)
	{
		if (!tweener->IsActive)
		{
			continue;
		}

		tweener->Update(deltaTime);
	}
}

UDYTweener* DYTweenManager::Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve)
{
	UDYTweener* tweener = getInactiveTweener();
	tweener->InitForUse(updateDelegate);
	tweener->Duration = duration;
	tweener->Curve = curve;
	return tweener;
}

UDYTweener* DYTweenManager::Tween(FTweenUpdateDelegate const& updateDelegate)
{
	UDYTweener* tweener = getInactiveTweener();
	tweener->InitForUse(updateDelegate);
	return tweener;
}

UDYTweener* DYTweenManager::Tween(TFunction<void(float)>&& updateCallback)
{
	UDYTweener* tweener = getInactiveTweener();
	tweener->InitForUse(MoveTemp(updateCallback));
	return tweener;
}

void DYTweenManager::SafeAbort(UDYTweener*& tweener, bool withEndCallback)
{
	if (tweener != nullptr)
	{
		if (withEndCallback)
		{
			tweener->AbortWithEndCallback();
		}
		else
		{
			tweener->AbortWithoutEndCallback();
		}
	}

	tweener = nullptr;
}

void DYTweenManager::Clear()
{
	m_Tweeners.Empty();
}

UDYTweener* DYTweenManager::getInactiveTweener()
{
	for (auto& tweener : m_Tweeners)
	{
		if (!tweener->IsActive)
		{
			return tweener;
		}
	}

	UDYTweener* newTweener = NewObject<UDYTweener>();
	newTweener->AddToRoot();
	m_Tweeners.Add(newTweener);

	return newTweener;
}