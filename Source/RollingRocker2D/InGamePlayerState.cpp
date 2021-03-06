// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInGamePlayerState::AInGamePlayerState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts
void AInGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Initialize player state
	m_CurrentLivesCount = m_InitialLivesCount;

	// Register player pawn events
	m_RollingRockerPawn = Cast<ARollingRockerPawn>(GetPlayerController()->GetPawn());
	if (!m_RollingRockerPawn->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerState's Pawn is not a RollingRockerPawn."));
		return;
	}

	m_RollingRockerPawn->OnDied.AddDynamic(this, &AInGamePlayerState::handleOnRollingRockerPawnDied);

	// Spawn respawn location selector
	FVector location(0.0f, 0.0f, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters spawnParams{ };
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	m_RespawnLocationSelector = Cast<AOnRodRespawnLocationSelector>(GetWorld()->SpawnActor(m_RespawnLocationSelectorType.Get(), &location, &rotation, spawnParams));
	if (!m_RespawnLocationSelector->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerState's m_RespawnLocationSelector is not spawned properly."));
		return;
	}

	m_RespawnLocationSelector->OnSelectionEnd.AddUniqueDynamic(this, &AInGamePlayerState::handleOnRespawnLocationSelectionEnd);
}

// Called every frame
void AInGamePlayerState::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!m_RespawnLocationSelector->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerState's m_RespawnLocationSelector is not spawned properly."));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, StaticEnum<EGameFlowState>()->GetValueAsString(m_GameFlowState));
	if (m_GameFlowState == EGameFlowState::WaitingForRespawn)
	{
		m_StateTimer += deltaTime;
		if (m_StateTimer < m_WaitingForRespawnTime)
		{
			return;
		}

		if (m_CurrentLivesCount > 0)
		{
			// Reduce one life and enter respawn state
			SetCurrentLivesCount(m_CurrentLivesCount - 1);

			// Reset Rod location
			auto rod = m_RollingRockerPawn->Rod;
			float avergeHeight = (rod->GetLeftEndHeight() + rod->GetRightEndHeight()) * 0.5f;
			rod->ResetLocationWithCustomHeight(avergeHeight);

			// Enable respawn location selector and setup timer
			m_RespawnLocationSelector->StartSelection(GetPlayerController(), rod);

			SetGameFlowState(EGameFlowState::Respawning);
			m_StateTimer = 0.0f;
		}
		else
		{
			// Show GameOver UI Menu
			auto gameOverMenu = CreateWidget(GetWorld(), m_GameOverMenuType);
			if (gameOverMenu->IsValidLowLevel())
			{
				gameOverMenu->AddToViewport();
			}

			auto playerController = GetPlayerController();
			playerController->SetInputMode(FInputModeGameAndUI());
			playerController->bShowMouseCursor = true;

			SetGameFlowState(EGameFlowState::GameOver);
		}
	}
	else if (m_GameFlowState == EGameFlowState::Respawning)
	{
		m_StateTimer += deltaTime;
		if (m_StateTimer < m_RespawningTime)
		{
			return;
		}

		// Respawn time is up! End the selection phase
		m_RespawnLocationSelector->EndSelection();
	}
}

void AInGamePlayerState::SetCurrentLivesCount(int newLivesCount)
{
	int prevLivesCount = m_CurrentLivesCount;
	m_CurrentLivesCount = newLivesCount;
	OnCurrentLivesCountChanged.Broadcast(prevLivesCount, m_CurrentLivesCount);
}

void AInGamePlayerState::SetGameFlowState(EGameFlowState playerState)
{
	EGameFlowState prevState = m_GameFlowState;
	m_GameFlowState = playerState;

	OnStateChanged.Broadcast(prevState, m_GameFlowState);
}

void AInGamePlayerState::handleOnRollingRockerPawnDied(FDeathEventData deathEventData)
{
	// Activate waiting for respawn state
	SetGameFlowState(EGameFlowState::WaitingForRespawn);
	m_StateTimer = 0.0f;
}


void AInGamePlayerState::handleOnRespawnLocationSelectionEnd(float locationOnRod)
{
	SetGameFlowState(EGameFlowState::Normal);
	m_StateTimer = 0.0f;

	m_RollingRockerPawn->RespawnRocker(locationOnRod, ERockerMovementState::Constrained);
}
