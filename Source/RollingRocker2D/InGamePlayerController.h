// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RollingRockerPawn.h"
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
	
private:
	ARollingRockerPawn* m_RollingRockerPawn = nullptr;

public:
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* inPawn) override;

private:
	UFUNCTION()
	void handleOnActivateFreeMoveModeActionPressed();

	UFUNCTION()
	void handleOnFreeMoveHorizontal(float axisValue);

	UFUNCTION()
	void handleOnMoveRodLeftEnd(float axisValue);

	UFUNCTION()
	void handleOnMoveRodRightEnd(float axisValue);

	UFUNCTION()
	void handleOnMoveRodBothEnds(float axisValue);
	
	UFUNCTION()
	void handleOnPauseActionPressed();
	
public:
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void ResumeGame();
};
