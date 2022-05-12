// Fill out your copyright notice in the Description page of Project Settings.


#include "DYTweenComponent.h"

// Sets default values for this component's properties
UDYTweenComponent::UDYTweenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDYTweenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDYTweenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	TweenManager.Update(DeltaTime);
}


UDYTweener* UDYTweenComponent::Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve)
{
	return TweenManager.Tween(updateDelegate, duration, curve);
}

UDYTweener* UDYTweenComponent::Tween(FTweenUpdateDelegate const& updateDelegate)
{
	return TweenManager.Tween(updateDelegate);
}

UDYTweener* UDYTweenComponent::Tween(TFunction<void(float)>&& updateCallback)
{
	return TweenManager.Tween(MoveTemp(updateCallback));
}