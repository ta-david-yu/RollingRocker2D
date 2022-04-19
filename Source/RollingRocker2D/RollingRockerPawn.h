// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Rod.h"
#include "Rocker.h"
#include "RollingRockerPawn.generated.h"

UCLASS()
class ROLLINGROCKER2D_API ARollingRockerPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class URocker> Rocker;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class URod> Rod;

	// Sets default values for this pawn's properties
	ARollingRockerPawn();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class USceneComponent> m_Root;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
