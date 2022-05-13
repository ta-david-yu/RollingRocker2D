// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DYTweenManager.generated.h"

DECLARE_DELEGATE_OneParam(FTweenUpdateDelegate, float);
DECLARE_DELEGATE(FTweenEndDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FTweenUpdateDynamicDelegate, float, progress);
DECLARE_DYNAMIC_DELEGATE(FTweenEndDynamicDelegate);

UCLASS(BlueprintType)
class DYTWEEN_API UDYTweener : public UObject
{
	GENERATED_BODY()

public:
	bool IsActive = false;
	bool IsPlaying = false;
	float Timer = 0.0f;
	float DelayTimer = 0.0f;
	UCurveFloat* Curve = nullptr;
    float Duration = 1.0f;

	FTweenUpdateDelegate UpdateDelegate{};
	FTweenEndDelegate EndDelegate{};

	FTweenUpdateDynamicDelegate UpdateDynamicDelegate;
	FTweenEndDynamicDelegate EndDynamicDelegate;

	TFunction<void(float)> UpdateCallback = nullptr;
	TFunction<void(void)> EndCallback = nullptr;

	UDYTweener() {};

	void InitForUse(FTweenUpdateDelegate const& updateDelegate);
	void InitForUse(FTweenUpdateDynamicDelegate const& updateDelegate);
	void InitForUse(TFunction<void(float)>&& updateCallback);
	void Update(float timeStep);
	
	UFUNCTION(BlueprintCallable, Category = "DYTween")
	void AbortWithEndCallback();
		
	UFUNCTION(BlueprintCallable, Category = "DYTween")
	void AbortWithoutEndCallback();

	UFUNCTION(BlueprintCallable, Category = "DYTween")
	UDYTweener* SetDuration(float duration) { Duration = duration; return this; }

	UFUNCTION(BlueprintCallable, Category = "DYTween")
	UDYTweener* SetCurve(UCurveFloat* curve) { Curve = curve; return this; }

	UFUNCTION(BlueprintCallable, Category = "DYTween")
	UDYTweener* SetUpdateDelegate(FTweenUpdateDynamicDelegate const& updateDelegate) { UpdateDynamicDelegate = FTweenUpdateDynamicDelegate{ updateDelegate }; return this; }

	UFUNCTION(BlueprintCallable, Category = "DYTween")
	UDYTweener* SetEndDelegate(FTweenEndDynamicDelegate const& endDelegate) { EndDynamicDelegate = FTweenEndDynamicDelegate{ endDelegate }; return this; }

	UDYTweener* SetUpdateDelegate(FTweenUpdateDelegate const& updateDelegate) { UpdateDelegate = FTweenUpdateDelegate{ updateDelegate }; return this; }
	UDYTweener* SetEndDelegate(FTweenEndDelegate const& endDelegate) { EndDelegate = FTweenEndDelegate{ endDelegate }; return this; }

	UDYTweener* SetUpdateDelegate(TFunction<void(float)>&& updateCallback) { UpdateCallback = MoveTemp(updateCallback); return this; }
	UDYTweener* SetEndDelegate(TFunction<void(void)>&& endCallback) { EndCallback = MoveTemp(endCallback); return this; }

	inline void ExecuteUpdate(float progress)
	{
		if (UpdateDelegate.IsBound())
		{
#if STATS
			TStatId StatId = TStatId();
			UObject* Object = UpdateDelegate.GetUObject();
			if (Object)
			{
				StatId = Object->GetStatID();
			}
			FScopeCycleCounter Context(StatId);
#endif
			UpdateDelegate.Execute(progress);
		}
		else if (UpdateDynamicDelegate.IsBound())
		{
			UpdateDynamicDelegate.Execute(progress);
		}
		else if (UpdateCallback)
		{
			UpdateCallback(progress);
		}
	}

	inline void ExecuteEnd()
	{
		if (EndDelegate.IsBound())
		{
#if STATS
			TStatId StatId = TStatId();
			UObject* Object = EndDelegate.GetUObject();
			if (Object)
			{
				StatId = Object->GetStatID();
			}
			FScopeCycleCounter Context(StatId);
#endif
			EndDelegate.Execute();
		}
		else if (EndDynamicDelegate.IsBound())
		{
			EndDynamicDelegate.Execute();
		}
		else if (EndCallback)
		{
			EndCallback();
		}
	}
};

class DYTweenManager
{
private:
	TArray<UDYTweener*> m_Tweeners;

public:
	// Called every frame
	void Update(float deltaTime);

public:
	UDYTweener* Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve);

	UDYTweener* Tween(FTweenUpdateDelegate const& updateDelegate);

	UDYTweener* Tween(TFunction<void(float)>&& updateCallback);

	void SafeAbort(UDYTweener*& tweener, bool withEndCallback = true);

	void Clear();

private:
	UDYTweener* getInactiveTweener();
};
