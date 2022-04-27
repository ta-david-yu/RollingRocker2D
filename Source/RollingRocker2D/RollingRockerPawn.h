// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Rod.h"
#include "Rocker.h"
#include "CanFall.h"
#include "RollingRockerPawn.generated.h"

USTRUCT(BlueprintType)
struct FMoveForwardEventData
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveAmount;
};

USTRUCT(BlueprintType)
struct FFallDownEventData
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveForwardEvent, FMoveForwardEventData, moveForwardEventData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFallDownEvent, FFallDownEventData, fallDownEventData);

UCLASS()
class ROLLINGROCKER2D_API ARollingRockerPawn : public APawn, public ICanFall
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URocker* Rocker = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* Rod = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RodAppearance = nullptr;

	UPROPERTY(BlueprintAssignable)
	FMoveForwardEvent OnMoveForward;
	
	UPROPERTY(BlueprintAssignable)
	FFallDownEvent OnFallDown;

protected:
	// Sets default values for this pawn's properties
	ARollingRockerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FallDownTo();									
	virtual void FallDownTo_Implementation() override; 

private:
	UFUNCTION()
	void handleOnRodLocationChanged(FRodLocationChangedEventData eventData);
};
