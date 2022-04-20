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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URocker : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* m_Rod = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USphereComponent* m_SphereCollision = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_LocationOnRod = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_OnRodGravity = 9.8f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_FreeMoveSpeed = 10.0f;

	// Velocity in constrained mode (non-free move mode)
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_CurrentVelocity = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool m_IsFreeMoveMode = false;

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

	UFUNCTION(BlueprintCallable)
	bool IsFreeMoveMode() const { return m_IsFreeMoveMode; }

	UFUNCTION(BlueprintCallable)
	void ActivateFreeMoveMode();

	UFUNCTION(BlueprintCallable)
	void DeactivateFreeMoveMode();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	  * Input move the rocker in free move mode manner
	  * @param	moveDirection: 1 means right, -1 means left
	  */
	UFUNCTION(BlueprintCallable)
	void FreeMove(float moveDirection);

	/**
	  * Move the rocker manually on the rod with clamping. 
	  * Return the delta value after clamping. If the new location exceeded the limit, the return value will be different from the input value.
	  */
	UFUNCTION(BlueprintCallable)
	float InstantMoveClamp(float locationDelta);
};
