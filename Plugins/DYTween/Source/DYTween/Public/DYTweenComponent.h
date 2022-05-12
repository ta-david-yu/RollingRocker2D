// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DYTweenManager.h"
#include "DYTweenComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DYTWEEN_API UDYTweenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DYTweenManager TweenManager{};

public:	
	// Sets default values for this component's properties
	UDYTweenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "DYTween|Component")
	UDYTweener* Tween(FTweenUpdateDynamicDelegate const& updateDelegate, float duration, UCurveFloat* curve);

	UDYTweener* Tween(FTweenUpdateDelegate const& updateDelegate);

	UDYTweener* Tween(TFunction<void(float)>&& updateCallback);
		
};
