// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"

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
	m_RespawnLocationSelector->SetActorHiddenInGame(true);
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

	if (!m_RespawnLocationSelector->IsSelecting())
	{
		return;
	}

	m_RespawnStateTimer += deltaTime;
	if (m_RespawnStateTimer > m_RespawnStateTime)
	{
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

void AInGamePlayerState::handleOnRollingRockerPawnDied(FDeathEventData deathEventData)
{
	// Reduce life
	int newLivesCount = m_CurrentLivesCount - 1;
	SetCurrentLivesCount(newLivesCount);

	// Reset Rod location
	m_RollingRockerPawn->Rod->ResetLocation();

	// Enable respawn location selector and setup timer
	if (!m_RespawnLocationSelector->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerState's m_RespawnLocationSelector is not spawned properly."));
		return;
	}
	m_RespawnLocationSelector->SetActorHiddenInGame(false);
	m_RespawnLocationSelector->StartSelection(GetPlayerController(), m_RollingRockerPawn->Rod);

	m_RespawnStateTimer = 0.0f;
}


void AInGamePlayerState::handleOnRespawnLocationSelectionEnd(float locationOnRod)
{
	m_RollingRockerPawn->RespawnRocker(locationOnRod, ERockerMovementState::Constrained);
}
