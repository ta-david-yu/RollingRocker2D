// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DYTweenManager.h"
#include "DYGlobalTween.generated.h"

/**
 * 
 */
UCLASS()
class DYTWEEN_API UDYGlobalTween : public UBlueprintFunctionLibrary, public FTickableGameObject
{
	GENERATED_BODY()

private:
	static DYTweenManager s_TweenManager;

public:
	virtual void Tick(float DeltaTime) override;

	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Always;
	}
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UDYGlobalTween, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const
	{
		return true;
	}
	virtual bool IsTickableInEditor() const
	{
		return false;
	}

public:
	UFUNCTION(BlueprintCallable, Category = "DYTween|Global", meta = (DisplayName = "Global Tween", Keywords = "Global Tween Animation"))
	static UDYTweener* Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve);

	static UDYTweener* Tween(FTweenUpdateDelegate const& updateDelegate);

	static UDYTweener* Tween(TFunction<void(float)>&& updateCallback);

	UFUNCTION(BlueprintCallable, Category = "DYTween|Global", meta = (DisplayName = "Clear All Global Tween", Keywords = "Global Tween Animation"))
	static void ClearAllTween();
};
