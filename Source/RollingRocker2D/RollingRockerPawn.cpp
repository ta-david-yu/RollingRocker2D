// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingRockerPawn.h"

// Sets default values
ARollingRockerPawn::ARollingRockerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARollingRockerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARollingRockerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARollingRockerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

