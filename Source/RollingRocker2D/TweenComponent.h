// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TweenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTweenUpdateEvent, float, progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTweenEvent);

USTRUCT(BlueprintType)
struct FTweenParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* Curve = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UTweenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* m_TweenCurve = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_Duration = 1.0f;

	UPROPERTY(BlueprintAssignable)
	FTweenEvent OnTweenBegin;

	UPROPERTY(BlueprintAssignable)
	FTweenUpdateEvent OnTweenUpdate;

	UPROPERTY(BlueprintAssignable)
	FTweenEvent OnTweenEnd;

	UPROPERTY(BlueprintAssignable)
	FTweenEvent OnTweenTerminated;

protected:
	UPROPERTY(BlueprintReadOnly)
	float m_Timer = 0.0f;
	
	UPROPERTY(BlueprintReadOnly)
	bool m_IsPlaying = false;

public:	
	// Sets default values for this component's properties
	UTweenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	UTweenComponent* Tween();

private:
	void play();
};
