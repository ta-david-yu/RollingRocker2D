// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RollingRockerPawn.h"
#include "OnRodRespawnLocationSelector.h"
#include "InGamePlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Normal UMETA(ToolTip = "The Rocker is on the rod, normal gameplay state"),
	WaitingForRespawn UMETA(ToolTip = "The state before the player can choose a respawn location"),
	Respawning UMETA(ToolTip = "The state when the player is choosing a respawn location"),
	GameOver UMETA(ToolTip = "The player has used up all the lives")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerStateChangedEvent, EPlayerState, prevState, EPlayerState, newState);
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
	float m_RespawningTime = 3;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_WaitingForRespawnTime = 2;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AOnRodRespawnLocationSelector> m_RespawnLocationSelectorType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> m_GameOverMenuType;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int m_CurrentLivesCount = 2;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EPlayerState m_PlayerState = EPlayerState::Normal;

public:
	UPROPERTY(BlueprintAssignable)
	FPlayerStateChangedEvent OnStateChanged;

	UPROPERTY(BlueprintAssignable)
	FLivesCountEvent OnCurrentLivesCountChanged;

private:
	ARollingRockerPawn* m_RollingRockerPawn = nullptr;
	
	AOnRodRespawnLocationSelector* m_RespawnLocationSelector = nullptr;

	float m_StateTimer = 0.0f;

public:
	AInGamePlayerState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetStateTimer() const { return m_StateTimer; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentLivesCount(int newLivesCount);

	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState playerState);
	
	UFUNCTION(BlueprintCallable)
	EPlayerState GetPlayerState() const { return m_PlayerState; }
private:
	UFUNCTION()
	void handleOnRollingRockerPawnDied(FDeathEventData deathEventData);

	
	UFUNCTION()
	void handleOnRespawnLocationSelectionEnd(float locationOnRod);
};
