// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto& pauseActionBinding = InputComponent->BindAction(TEXT("TogglePause"), IE_Pressed, this, &AInGamePlayerController::handleOnPauseActionPressed);
	pauseActionBinding.bExecuteWhenPaused = true;
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