// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto& pauseActionBinding = InputComponent->BindAction(TEXT("TogglePause"), IE_Pressed, this, &AInGamePlayerController::handleOnPauseActionPressed);
	pauseActionBinding.bExecuteWhenPaused = true;

	InputComponent->BindAxis(TEXT("MoveRodLeftEnd"), this, &AInGamePlayerController::handleOnMoveRodLeftEnd);
	InputComponent->BindAxis(TEXT("MoveRodRightEnd"), this, &AInGamePlayerController::handleOnMoveRodRightEnd);

	InputComponent->BindAction(TEXT("ActivateFreeMoveMode"), IE_Pressed, this, &AInGamePlayerController::handleOnActivateFreeMoveModeActionPressed);
	InputComponent->BindAxis(TEXT("FreeMoveHorizontal"), this, &AInGamePlayerController::handleOnFreeMoveHorizontal);
	InputComponent->BindAxis(TEXT("MoveRodBothEnds"), this, &AInGamePlayerController::handleOnMoveRodBothEnds);
}

void AInGamePlayerController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
	m_RollingRockerPawn = Cast<ARollingRockerPawn>(inPawn);
	if (!m_RollingRockerPawn->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Possessed Pawn is not a RollingRockerPawn."));
		return;
	}
}

void AInGamePlayerController::handleOnPauseActionPressed()
{
	if (m_IsGamePaused)
	{
		ResumeGame();
	}
	else
	{
		PauseGame();
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("PAUSE PRESSED"));
}

void AInGamePlayerController::handleOnActivateFreeMoveModeActionPressed()
{
	auto rocker = m_RollingRockerPawn->Rocker;
	if (rocker->IsMovementState(ERockerMovementState::External))
	{
		return;
	}

	if (rocker->IsMovementState(ERockerMovementState::Constrained))
	{
		rocker->SetMovementState(ERockerMovementState::Free);
	}
	else if (rocker->IsMovementState(ERockerMovementState::Free))
	{
		rocker->SetMovementState(ERockerMovementState::Constrained);
	}
}

void AInGamePlayerController::handleOnFreeMoveHorizontal(float axisValue)
{
	auto rocker = m_RollingRockerPawn->Rocker;
	if (rocker->IsMovementState(ERockerMovementState::Free))
	{
		rocker->FreeMove(axisValue);
	}
}

void AInGamePlayerController::handleOnMoveRodLeftEnd(float axisValue)
{
	auto rocker = m_RollingRockerPawn->Rocker;
	if (rocker->IsMovementState(ERockerMovementState::Constrained))
	{
		m_RollingRockerPawn->Rod->MoveLeftEnd(axisValue);
	}
}

void AInGamePlayerController::handleOnMoveRodRightEnd(float axisValue)
{
	auto rocker = m_RollingRockerPawn->Rocker;
	if (rocker->IsMovementState(ERockerMovementState::Constrained))
	{
		m_RollingRockerPawn->Rod->MoveRightEnd(axisValue);
	}
}

void AInGamePlayerController::handleOnMoveRodBothEnds(float axisValue)
{
	auto rocker = m_RollingRockerPawn->Rocker;
	if (rocker->IsMovementState(ERockerMovementState::Free) || m_RollingRockerPawn->IsDead())
	{
		m_RollingRockerPawn->Rod->MoveLeftEnd(axisValue);
		m_RollingRockerPawn->Rod->MoveRightEnd(axisValue);
	}
}

void AInGamePlayerController::PauseGame()
{
	// Show pause menu
	m_CurrentPauseMenuInstance = CreateWidget(GetWorld(), m_PauseMenuType);
	if (m_CurrentPauseMenuInstance->IsValidLowLevel())
	{
		m_CurrentPauseMenuInstance->AddToViewport();
	}

	SetInputMode(FInputModeGameAndUI());
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	bShowMouseCursor = true;

	m_IsGamePaused = true;
	OnPaused.Broadcast();
}

void AInGamePlayerController::ResumeGame()
{
	// Remove pause menu
	if (m_CurrentPauseMenuInstance->IsValidLowLevel())
	{
		m_CurrentPauseMenuInstance->RemoveFromViewport();
		m_CurrentPauseMenuInstance = nullptr;
	}

	SetInputMode(FInputModeGameOnly());
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	bShowMouseCursor = false;

	m_IsGamePaused = false;
	OnResumed.Broadcast();
}