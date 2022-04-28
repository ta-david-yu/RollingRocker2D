// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RollingRockerPawn.h"
#include "InGamePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLivesCountEvent, int, prevLivesCount, int, newLivesCount);

UCLASS()
class ROLLINGROCKER2D_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int m_MaxLivesCount = 4;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int m_CurrentLivesCount = 4;

public:
	UPROPERTY(BlueprintAssignable)
	FLivesCountEvent OnCurrentLivesCountChanged;

private:
	ARollingRockerPawn* m_RollingRockerPawn = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentLivesCount(int newLivesCount);

private:
	UFUNCTION()
	void handleOnRollingRockerPawnDied(FDeathEventData deathEventData);
};
