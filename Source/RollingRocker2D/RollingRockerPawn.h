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
	URocker* Rocker;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	URod* Rod;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* m_Root;

	// Sets default values for this pawn's properties
	ARollingRockerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
