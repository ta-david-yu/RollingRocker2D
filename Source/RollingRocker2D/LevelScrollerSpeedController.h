// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScrollActorLocationOnMoveForward.h"
#include "LevelScrollerSpeedController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API ULevelScrollerSpeedController : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UScrollActorLocationOnMoveForward> m_Scroller = nullptr;

private:
	bool m_IsScrolling = true;
	float m_TimeToChangeState = 1.0f;

public:	
	// Sets default values for this component's properties
	ULevelScrollerSpeedController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void SlowDown(float timeToFullySlowDown = 1.0f);
	
	UFUNCTION(BlueprintCallable)
	void SpeedUp(float timeToFullySpeedUp = 1.0f);
};
