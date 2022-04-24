// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollActorLocationOnMoveForward.h"

// Sets default values for this component's properties
UScrollActorLocationOnMoveForward::UScrollActorLocationOnMoveForward()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UScrollActorLocationOnMoveForward::HandleOnMoveForward(FMoveForwardEventData moveForwardEventData)
{
	FVector movement = -FVector::UpVector * moveForwardEventData.MoveAmount * m_ScrollSpeedMultiplier;
	Scroll(movement);
}

void UScrollActorLocationOnMoveForward::Scroll(FVector movement)
{
	AActor* parentActor = GetOwner();
	FVector newLocation = parentActor->GetActorLocation() + movement;
	parentActor->SetActorLocation(newLocation, true);
}