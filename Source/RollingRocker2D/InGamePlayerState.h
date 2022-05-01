// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RollingRockerPawn.h"
#include "OnRodRespawnLocationSelector.h"
#include "InGamePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLivesCountEvent, int, prevLivesCount, int, newLivesCount);

UCLASS()
class ROLLINGROCKER2D_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int m_MaxLivesCount = 4;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int m_InitialLivesCount = 2;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_RespawnStateTime = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AOnRodRespawnLocationSelector> m_RespawnLocationSelectorType;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int m_CurrentLivesCount = 2;

public:
	UPROPERTY(BlueprintAssignable)
	FLivesCountEvent OnCurrentLivesCountChanged;

private:
	ARollingRockerPawn* m_RollingRockerPawn = nullptr;
	
	AOnRodRespawnLocationSelector* m_RespawnLocationSelector = nullptr;

	float m_RespawnStateTimer = 0.0f;
public:
	AInGamePlayerState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentLivesCount(int newLivesCount);

private:
	UFUNCTION()
	void handleOnRollingRockerPawnDied(FDeathEventData deathEventData);

	
	UFUNCTION()
	void handleOnRespawnLocationSelectionEnd(float locationOnRod);
};
