// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingRockerPawn.h"

// Sets default values
ARollingRockerPawn::ARollingRockerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_FallAnimationController = CreateDefaultSubobject<UFallAnimationController>("FallAnimationController");
}

// Called when the game starts or when spawned
void ARollingRockerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!Rod->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("RollingRockerPawn's Rod is not valid."));
		return;
	}

	if (!Rocker->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("RollingRockerPawn's Rocker is not valid."));
		return;
	}

	// Register Events
	Rod->OnRodLocationChanged.AddDynamic(this, &ARollingRockerPawn::handleOnRodLocationChanged);
	Rocker->OnMovementStateChanged.AddDynamic(this, &ARollingRockerPawn::handleOnRockerMovementStateChanged);
}

// Called to bind functionality to input
void ARollingRockerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARollingRockerPawn::FallDownTo_Implementation(FFallDownData fallDownData)
{
	SetActorEnableCollision(false);
	Rocker->SetMovementState(ERockerMovementState::External);

	m_FallAnimationController->PlayFallAnimationOnSceneComponent(Rocker, Rocker->GetVelocity3D(), Rocker->GetLastRotationDelta(), 50.0f);
	
	// Setup a delayed kill that will happen when the fall animation is done
	m_FallAnimationController->OnAnimationEnd.AddDynamic(this, &ARollingRockerPawn::handleOnFallAnimationEnd);

	OnFallDown.Broadcast(FFallDownEventData{});
}

FKillResult ARollingRockerPawn::Kill_Implementation()
{
	m_IsDead = true;
	OnDied.Broadcast(FDeathEventData { });
	
	FKillResult result { };
	result.Success = true;

	return result;
}

void ARollingRockerPawn::RespawnRocker(float pointOnRod, ERockerMovementState initialMovementState)
{
	Rocker->SetLocationOnRod(pointOnRod);

	FVector worldLocation = Rod->GetWorldLocationFromPointOnRod(pointOnRod) + FVector::UpVector * Rocker->GetCollisionRadius();
	Rocker->SetWorldLocation(worldLocation);
	Rocker->SetWorldScale3D(FVector::OneVector);

	SetActorEnableCollision(true);
	Rocker->SetMovementState(initialMovementState);

	Rocker->SetConstrainedModeVelocity(0);
}

void ARollingRockerPawn::handleOnFallAnimationEnd(USceneComponent* targetSceneComponent)
{
	Kill();
	m_FallAnimationController->OnAnimationEnd.RemoveDynamic(this, &ARollingRockerPawn::handleOnFallAnimationEnd);
}

void ARollingRockerPawn::handleOnRodLocationChanged(FRodLocationChangedEventData eventData)
{
	// Do move forward logic
	float leftExceededAmount = eventData.LeftExceededHeightAmount;
	float rightExceededAmount = eventData.RightExceededHeightAmount;

	if (leftExceededAmount <= 0 && rightExceededAmount <= 0)
	{
		// No exceeding happens, early out.
		return;
	}

	FMoveForwardEventData data{ };
	data.MoveAmount = leftExceededAmount > rightExceededAmount ? leftExceededAmount : rightExceededAmount;
	OnMoveForward.Broadcast(data);
}

void ARollingRockerPawn::handleOnRockerMovementStateChanged(ERockerMovementState prevMovementState, ERockerMovementState nextMovementState)
{
	if (nextMovementState == ERockerMovementState::Free)
	{
		Rod->ResetLocation();
	}
}
