// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Rod.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRodLocationChangedEvent, FVector, NewLeftLocation, FVector, NewRightLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRodEndLocationChangedEvent, FVector, NewLocation);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URod : public USceneComponent
{
	GENERATED_BODY()

protected:
	// If the value is 50, meaning the movable location ranging from -25 to 25
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float m_LeftEndMovableRange = 50;
	
	// If the value is 50, meaning the movable location ranging from -25 to 25
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float m_RightEndMovableRange = 50;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "300.0", UIMin = "0", UIMax = "300.0"))
	float m_MovableAreaWidth = 200;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "50.0", UIMin = "0", UIMax = "50.0"))
	float m_EndMaxSpeed = 10;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_LeftEndHeight = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_RightEndHeight = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FRodEndLocationChangedEvent OnLeftEndLocationChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRodEndLocationChangedEvent OnRightEndLocationChanged;

	UPROPERTY(BlueprintAssignable)
	FRodLocationChangedEvent OnRodLocationChanged;

private:
	UPROPERTY(VisibleAnywhere)
	float m_LeftEndMoveBuffer = 0;

	UPROPERTY(VisibleAnywhere)
	float m_RightEndMoveBuffer = 0;

public:
	// Sets default values for this component's properties
	URod();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	float GetLeftEndHeight() const { return m_LeftEndHeight; }

	UFUNCTION(BlueprintCallable)
	float GetRightEndHeight() const { return m_RightEndHeight; }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FVector GetLeftEndLocation() const { return GetComponentLocation() - FVector::ForwardVector * m_MovableAreaWidth * 0.5f + FVector::UpVector * m_LeftEndHeight; }
	
	UFUNCTION(BlueprintCallable)
	FVector GetRightEndLocation() const { return GetComponentLocation() + FVector::ForwardVector * m_MovableAreaWidth * 0.5f + FVector::UpVector * m_RightEndHeight; }

	UFUNCTION(BlueprintCallable)
	FVector GetRodVector() const { return GetRightEndLocation() - GetLeftEndLocation(); }

	UFUNCTION(BlueprintCallable)
	void MoveLeftEnd(float scalar);
	
	UFUNCTION(BlueprintCallable)
	void MoveRightEnd(float scalar);
};
