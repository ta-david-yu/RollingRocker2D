// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Rod.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Rocker.generated.h"

UENUM(BlueprintType)
enum class ERockerMovementState : uint8
{
	Constrained UMETA(ToolTip="Move by on rod gravity acceleration"),
	Free UMETA(ToolTip="Move left or right freely without influences of gravity"),
	External UMETA(ToolTip="No movement is applied, can be used to play external force or simply freeze Rocker in location")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URocker : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* m_Rod = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USphereComponent* m_SphereCollision = nullptr;
	
	UPROPERTY
	(
		BlueprintReadOnly, EditAnywhere, Category = "Rocker Movement|Constrained Mode",
		meta = (ClampMin = "0", ClampMax = "10.0", UIMin = "0", UIMax = "10.0")
	)
	float m_TimeToReachConstrainedMaxSpeedWithMaxAngle = 1.5f;

	UPROPERTY
	(
		BlueprintReadOnly, EditAnywhere, Category = "Rocker Movement|Constrained Mode",
		meta = (ClampMin = "0", ClampMax = "200.0", UIMin = "0", UIMax = "200.0")
	)
	float m_ConstrainedMaxSpeed = 100.0f;
	
	UPROPERTY
	(
		BlueprintReadOnly, EditAnywhere, Category = "Rocker Movement|Constrained Mode",
		meta = (ClampMin = "0", ClampMax = "45.0", UIMin = "0", UIMax = "45.0")
	)
	float m_MinimumAccelerationAngle = 5.0f;

	UPROPERTY
	(
		BlueprintReadOnly, EditAnywhere, Category = "Rocker Movement|Constrained Mode",
		meta = (ClampMin = "0.1", ClampMax = "10.0", UIMin = "0.1", UIMax = "10.0")
	)
	float m_TimeToCompletelyStopInConstrainedMode = 1.5f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Rocker Movement|Constrained Mode")
	float m_OnRodGravity = 196.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Rocker Movement|Constrained Mode")
	float m_OnRodDeceleration = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rocker Movement|FreeMove Mode")
	float m_FreeMoveSpeed = 100.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_LocationOnRod = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ERockerMovementState m_MovementState = ERockerMovementState::Constrained;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_CurrentVelocity = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator m_LastRotationDelta;

private:
	UPROPERTY(VisibleAnywhere)
	float m_FreeMoveDirectionBuffer = 0;

public:	
	// Sets default values for this component's properties
	URocker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float calculateConstrainedModeAccelerationOnSlope(FVector groundDirection, FVector slopeDirection) const;

public:
	UFUNCTION(BlueprintCallable)
	void SetRod(URod* rod);

	UFUNCTION(BlueprintCallable)
	float GetCollisionRadius() const { return m_SphereCollision->GetUnscaledSphereRadius(); }

	UFUNCTION(BlueprintCallable)
	float GetLocationOnRod() const { return m_LocationOnRod; }

	UFUNCTION(BlueprintCallable)
	void SetLocationOnRod(float locationOnRod) { m_LocationOnRod = locationOnRod; }

	UFUNCTION(BlueprintCallable)
	FVector GetVelocity3D() const { return m_CurrentVelocity * m_Rod->GetRodVector().GetUnsafeNormal(); }

	UFUNCTION(BlueprintCallable)
	FRotator GetAngularVelocity(float locationDelta) const;
	
	UFUNCTION(BlueprintCallable)
	FRotator GetLastRotationDelta() const { return m_LastRotationDelta; }

	UFUNCTION(BlueprintCallable)
	void SetMovementState(ERockerMovementState state);

	UFUNCTION(BlueprintCallable)
	bool IsMovementState(ERockerMovementState state) const { return m_MovementState == state; }

	UFUNCTION(BlueprintCallable)
	void SetConstrainedModeVelocity(float velocity) { m_CurrentVelocity = 0; }

	/**
	  * Input move the rocker in free move mode manner
	  * @param	moveDirection: 1 means right, -1 means left
	  */
	UFUNCTION(BlueprintCallable)
	void FreeMove(float moveDirection);

	/**
	  * Rotate the transform as if Rocker has moved the given locationDelta value.
	  */
	UFUNCTION(BlueprintCallable)
	void RotateWithLocationDelta(float locationDelta);

	/**
	  * Move the rocker manually on the rod with clamping. 
	  * Return the delta value after clamping. If the new location exceeded the limit, the return value will be different from the input value.
	  */
	UFUNCTION(BlueprintCallable)
	float InstantMoveClamp(float locationDelta);

	UFUNCTION(BlueprintCallable)
	void SnapWorldLocationToRod();

	UFUNCTION(BlueprintCallable)
	float GetEvaluatedGravityFromTimeToReachMaxSpeed(float rodMovableAreaWidth, float rodEndMovableRange);


private:
	UFUNCTION()
	void handleOnRodLocationChanged(FRodLocationChangedEventData eventData);

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif
};
