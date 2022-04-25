// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseEvent);
/**
 * 
 */
UCLASS()
class ROLLINGROCKER2D_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> m_PauseMenuType;

	UPROPERTY()
	bool m_IsGamePaused = false;

	UPROPERTY()
	UUserWidget* m_CurrentPauseMenuInstance;
	
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere)
	FPauseEvent OnPaused;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere)
	FPauseEvent OnResumed;
	
public:
	virtual void SetupInputComponent() override;

private:
	void handleOnPauseActionPressed();

public:
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void ResumeGame();
};
