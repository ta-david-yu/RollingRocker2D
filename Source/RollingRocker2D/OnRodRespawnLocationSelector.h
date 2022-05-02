// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnRodSceneComponent.h"
#include "Rod.h"
#include "OnRodRespawnLocationSelector.generated.h"

USTRUCT(BlueprintType)
struct FSelectionStartEventData
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInGamePlayerState* PlayerState;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectionStartEvent, FSelectionStartEventData, selectionStartEventData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectionEndEvent, float, locationOnRod);

UCLASS(BlueprintType)
class ROLLINGROCKER2D_API AOnRodRespawnLocationSelector : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UOnRodSceneComponent> m_OnRodScene;

public:
	UPROPERTY(BlueprintAssignable)
	FSelectionStartEvent OnSelectionStart;
	
	UPROPERTY(BlueprintAssignable)
	FSelectionEndEvent OnSelectionEnd;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_FreeMoveSpeed = 100.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool m_IsSelecting = false;

private:
	UPROPERTY(VisibleAnywhere)
	float m_FreeMoveDirectionBuffer = 0;

	APlayerController* m_PlayerController;

public:	
	// Sets default values for this actor's properties
	AOnRodRespawnLocationSelector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsSelecting() const { return m_IsSelecting; }

	UFUNCTION(BlueprintCallable)
	void StartSelection(APlayerController* playerController, URod* rod);
	
	UFUNCTION(BlueprintCallable)
	void EndSelection();

private:
	UFUNCTION()
	void handleOnFreeMoveHorizontal(float axisValue);

	UFUNCTION()
	void handleOnConfirmRespawnLocationActionPressed();
};
