// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RollingRockerPawn.h"
#include "ScrollActorLocationOnMoveForward.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UScrollActorLocationOnMoveForward : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "10.0", UIMin = "0", UIMax = "10.0"))
	float m_MoveForwardScrollSpeedMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool m_IsAutoScrolling = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float m_InitialAutoScrollSpeed = 10.0f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float m_CurrentAutoScrollSpeed = 10.0f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float m_AutoScrollSpeedMultiplier = 1.0f;

public:	
	// Sets default values for this component's properties
	UScrollActorLocationOnMoveForward();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	float GetAutoScrollSpeedMultiplier() const { return m_AutoScrollSpeedMultiplier; }

	UFUNCTION(BlueprintCallable)
	void SetAutoScrollSpeedMultiplier(float multiplier) { m_AutoScrollSpeedMultiplier = multiplier; }

	UFUNCTION(BlueprintCallable)
	void HandleOnMoveForward(FMoveForwardEventData moveForwardEventData);

	UFUNCTION(BlueprintCallable)
	void Scroll(FVector movement);
};
