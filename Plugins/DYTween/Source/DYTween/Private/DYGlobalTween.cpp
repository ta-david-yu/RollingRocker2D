// Fill out your copyright notice in the Description page of Project Settings.


#include "DYGlobalTween.h"

DYTweenManager UDYGlobalTween::s_TweenManager = DYTweenManager{};

void UDYGlobalTween::Tick(float DeltaTime)
{
	s_TweenManager.Update(DeltaTime);
}

UDYTweener* UDYGlobalTween::Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve)
{
	return s_TweenManager.Tween(updateDelegate, duration, curve);
}

UDYTweener* UDYGlobalTween::Tween(FTweenUpdateDelegate const& updateDelegate)
{
	return s_TweenManager.Tween(updateDelegate);
}

UDYTweener* UDYGlobalTween::Tween(TFunction<void(float)>&& updateCallback)
{
	return s_TweenManager.Tween(MoveTemp(updateCallback));
}

void UDYGlobalTween::ClearAllTween()
{
	s_TweenManager.Clear();
}