// Fill out your copyright notice in the Description page of Project Settings.


#include "OnRodRespawnLocationSelector.h"

// Sets default values
AOnRodRespawnLocationSelector::AOnRodRespawnLocationSelector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_OnRodScene = CreateDefaultSubobject<UOnRodSceneComponent>("OnRodSceneRoot");
	SetRootComponent(m_OnRodScene);
}

// Called when the game starts or when spawned
void AOnRodRespawnLocationSelector::BeginPlay()
{
	Super::BeginPlay();

	// Bind input
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();

	InputComponent->BindAction(TEXT("ConfirmRespawnLocation"), IE_Pressed, this, &AOnRodRespawnLocationSelector::handleOnConfirmRespawnLocationActionPressed);
	InputComponent->BindAxis(TEXT("FreeMoveHorizontal"), this, &AOnRodRespawnLocationSelector::handleOnFreeMoveHorizontal);
}

// Called every frame
void AOnRodRespawnLocationSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_IsSelecting)
	{
		return;
	}

	// Move selector location
	float velocity = m_FreeMoveDirectionBuffer * m_FreeMoveSpeed;
	if (!FMath::IsNearlyEqual(m_FreeMoveDirectionBuffer, 0))
	{
		m_FreeMoveDirectionBuffer = 0;
	}

	float locationOnRodDelta = velocity * DeltaTime;

	if (FMath::Abs(locationOnRodDelta) > 0)
	{
		// Move
		float actualAppliedDelta = m_OnRodScene->InstantMoveClamp(locationOnRodDelta);
	}
}

void AOnRodRespawnLocationSelector::StartSelection(APlayerController* playerController, URod* rod)
{
	m_IsSelecting = true;

	m_PlayerController = playerController;

	EnableInput(playerController);

	m_OnRodScene->SetRod(rod);
	m_OnRodScene->SetLocationOnRod(0);
	m_OnRodScene->SnapWorldLocationToRod();

	OnSelectionStart.Broadcast(0);
}

void AOnRodRespawnLocationSelector::EndSelection()
{
	m_IsSelecting = false;

	DisableInput(m_PlayerController);
	OnSelectionEnd.Broadcast(m_OnRodScene->GetLocationOnRod());
}


void AOnRodRespawnLocationSelector::handleOnFreeMoveHorizontal(float axisValue)
{
	m_FreeMoveDirectionBuffer = FMath::Clamp(axisValue, -1, 1);
}

void AOnRodRespawnLocationSelector::handleOnConfirmRespawnLocationActionPressed()
{
	if (!m_IsSelecting)
	{
		return;
	}

	EndSelection();
}
