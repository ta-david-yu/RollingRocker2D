// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"

// Called when the game starts
void AInGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Initialize player state
	m_CurrentLivesCount = m_MaxLivesCount;

	// Register player pawn events
	m_RollingRockerPawn = Cast<ARollingRockerPawn>(GetPlayerController()->GetPawn());
	if (!m_RollingRockerPawn->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerState's Pawn is not a RollingRockerPawn."));
		return;
	}

	m_RollingRockerPawn->OnDied.AddDynamic(this, &AInGamePlayerState::handleOnRollingRockerPawnDied);
}

void AInGamePlayerState::SetCurrentLivesCount(int newLivesCount)
{
	int prevLivesCount = m_CurrentLivesCount;
	m_CurrentLivesCount = newLivesCount;
	OnCurrentLivesCountChanged.Broadcast(prevLivesCount, m_CurrentLivesCount);
}

void AInGamePlayerState::handleOnRollingRockerPawnDied(FDeathEventData deathEventData)
{
	int newLivesCount = m_CurrentLivesCount - 1;
	SetCurrentLivesCount(newLivesCount);
}