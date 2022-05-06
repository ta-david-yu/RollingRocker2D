// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TweenManagerComponent.generated.h"

DECLARE_DELEGATE_OneParam(FTweenUpdateDelegate, float);
DECLARE_DELEGATE(FTweenEndDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FTweenUpdateDynamicDelegate, float, progress);
DECLARE_DYNAMIC_DELEGATE(FTweenEndDynamicDelegate);

UCLASS(BlueprintType)
class ROLLINGROCKER2D_API UTweener : public UObject
{
	GENERATED_BODY()

public:
	bool IsActive = false;
	bool IsPlaying = false;
	float Timer = 0.0f;
	float DelayTimer = 0.0f;
	UCurveFloat* Curve = nullptr;
    float Duration = 1.0f;

	FTweenUpdateDelegate UpdateDelegate;
	FTweenEndDelegate EndDelegate;

	FTweenUpdateDynamicDelegate UpdateDynamicDelegate;
	FTweenEndDynamicDelegate EndDynamicDelegate;

	TFunction<void(float)> UpdateCallback = nullptr;
	TFunction<void(void)> EndCallback = nullptr;

	UTweener() {};

	void InitForUse(FTweenUpdateDelegate const& updateDelegate);
	void InitForUse(FTweenUpdateDynamicDelegate const& updateDelegate);
	void InitForUse(TFunction<void(float)>&& updateCallback);
	void Update(float timeStep);
	
	UFUNCTION(BlueprintCallable)
	void AbortWithEndCallback();
		
	UFUNCTION(BlueprintCallable)
	void AbortWithoutEndCallback();

	UFUNCTION(BlueprintCallable)
	UTweener* SetDuration(float duration) { Duration = duration; return this; }

	UFUNCTION(BlueprintCallable)
	UTweener* SetCurve(UCurveFloat* curve) { Curve = curve; return this; }

	UFUNCTION(BlueprintCallable)
	UTweener* SetUpdateDelegate(FTweenUpdateDynamicDelegate const& updateDelegate) { UpdateDynamicDelegate = FTweenUpdateDynamicDelegate{ updateDelegate }; return this; }

	UFUNCTION(BlueprintCallable)
	UTweener* SetEndDelegate(FTweenEndDynamicDelegate const& endDelegate) { EndDynamicDelegate = FTweenEndDynamicDelegate{ endDelegate }; return this; }

	UTweener* SetUpdateDelegate(FTweenUpdateDelegate const& updateDelegate) { UpdateDelegate = FTweenUpdateDelegate{ updateDelegate }; return this; }
	UTweener* SetEndDelegate(FTweenEndDelegate const& endDelegate) { EndDelegate = FTweenEndDelegate{ endDelegate }; return this; }

	UTweener* SetUpdateDelegate(TFunction<void(float)>&& updateCallback) { UpdateCallback = MoveTemp(updateCallback); return this; }
	UTweener* SetEndDelegate(TFunction<void(void)>&& endCallback) { EndCallback = MoveTemp(endCallback); return this; }

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UTweenManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<UTweener*> m_Tweeners;

public:	
	// Sets default values for this component's properties
	UTweenManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	UTweener* Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve);

	UTweener* Tween(FTweenUpdateDelegate const& updateDelegate);

	UTweener* Tween(TFunction<void(float)>&& updateCallback);

private:
	UTweener* getInactiveTweener();
};
