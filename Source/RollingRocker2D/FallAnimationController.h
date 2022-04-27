// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "FallAnimationController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UFallAnimationController : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ScaleToZeroTime = 1.0f;

	// TODO: Reference a Curve Data
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCurveFloat> ScalingCurve = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool m_IsAnimating = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_ScaleTimer = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* m_TargetSceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector m_Velocity = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_Gravity = 0.0f;

public:	
	// Sets default values for this component's properties
	UFallAnimationController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PlayFallAnimationOnSceneComponent(USceneComponent* sceneComponent, FVector initialVelocity, float gravity);

	UFUNCTION(BlueprintCallable)
	void StopFallAnimation();
};
