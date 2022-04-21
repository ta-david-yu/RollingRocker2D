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
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URocker* Rocker = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* Rod = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RodAppearance = nullptr;

protected:
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
private:
	
	UFUNCTION()
	void HandleOnRodLocationChanged(FVector leftEndLocation, FVector rightEndLocation);
};
