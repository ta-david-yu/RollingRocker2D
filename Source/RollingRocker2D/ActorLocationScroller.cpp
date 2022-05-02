// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorLocationScroller.h"

// Sets default values for this component's properties
UActorLocationScroller::UActorLocationScroller()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorLocationScroller::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_CurrentSpeed = m_InitialSpeed;
}


// Called every frame
void UActorLocationScroller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto movement = FVector::UpVector * DeltaTime* m_SpeedMultiplier * m_CurrentSpeed;

	AActor* parentActor = GetOwner();
	FVector newLocation = parentActor->GetActorLocation() + movement;
	parentActor->SetActorLocation(newLocation, m_DoSweep);
}

