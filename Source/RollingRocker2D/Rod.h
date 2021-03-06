// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Rod.generated.h"

USTRUCT(BlueprintType)
struct FRodLocationChangedEventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OldLeftLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OldRightLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector NewLeftLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector NewRightLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftExceededHeightAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightExceededHeightAmount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRodLocationChangedEvent, FRodLocationChangedEventData, rodLocationChangedData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRodEndLocationChangedEvent, FVector, NewLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetLocationEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URod : public USceneComponent
{
	GENERATED_BODY()

protected:
	// If the value is 50, meaning the movable location ranging from -25 to 25
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float m_EndMovableRange = 50;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "300.0", UIMin = "0", UIMax = "300.0"))
	float m_MovableAreaWidth = 200;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "50.0", UIMin = "0", UIMax = "50.0"))
	float m_EndMaxSpeed = 10;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "5.0", UIMin = "0", UIMax = "5.0"))
	float m_ResetLocationHalfLifeTime = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "-50.0", ClampMax = "50.0", UIMin = "-50.0", UIMax = "50.0"))
	float m_DefaultResetLocationHeight = 0.0f;

public:
	UPROPERTY(BlueprintAssignable)
	FRodEndLocationChangedEvent OnLeftEndLocationChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRodEndLocationChangedEvent OnRightEndLocationChanged;

	UPROPERTY(BlueprintAssignable)
	FRodLocationChangedEvent OnRodLocationChanged;
	
	UPROPERTY(BlueprintAssignable)
	FResetLocationEvent OnResetLocationBegin;
	
	UPROPERTY(BlueprintAssignable)
	FResetLocationEvent OnResetLocationEnd;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_LeftEndHeight = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_RightEndHeight = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool m_IsResettingLocation = false;

	UPROPERTY(VisibleAnywhere)
	float m_ResetLocationHeight = 0;

	UPROPERTY(VisibleAnywhere)
	float m_LeftEndMoveBuffer = 0;

	UPROPERTY(VisibleAnywhere)
	float m_RightEndMoveBuffer = 0;

public:
	// Sets default values for this component's properties
	URod();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetEndMovableRange() const { return m_EndMovableRange; }

	UFUNCTION(BlueprintCallable)
	float GetMovableAreaWidth() const { return m_MovableAreaWidth; }

	UFUNCTION(BlueprintCallable)
	float GetLeftEndHeight() const { return m_LeftEndHeight; }

	UFUNCTION(BlueprintCallable)
	float GetRightEndHeight() const { return m_RightEndHeight; }

	UFUNCTION(BlueprintCallable)
	FVector GetWorldLocationFromPointOnRod(float pointOnRod) const;

	UFUNCTION(BlueprintCallable)
	FVector GetLeftEndLocation() const { return GetComponentLocation() - FVector::ForwardVector * m_MovableAreaWidth * 0.5f + FVector::UpVector * m_LeftEndHeight; }
	
	UFUNCTION(BlueprintCallable)
	FVector GetRightEndLocation() const { return GetComponentLocation() + FVector::ForwardVector * m_MovableAreaWidth * 0.5f + FVector::UpVector * m_RightEndHeight; }

	UFUNCTION(BlueprintCallable)
	FVector GetRodCenterLocation() const { return (GetLeftEndLocation() + GetRightEndLocation()) * 0.5f; }

	UFUNCTION(BlueprintCallable)
	FVector GetRodVector() const { return GetRightEndLocation() - GetLeftEndLocation(); }

	UFUNCTION(BlueprintCallable)
	double GetAngle() const;

	UFUNCTION(BlueprintCallable)
	double GetSignedAngle() const;

	UFUNCTION(BlueprintCallable)
	void MoveLeftEnd(float scalar);
	
	UFUNCTION(BlueprintCallable)
	void MoveRightEnd(float scalar);

	UFUNCTION(BlueprintCallable)
	void ResetLocation();
	
	UFUNCTION(BlueprintCallable)
	void ResetLocationWithCustomHeight(float height);
};
