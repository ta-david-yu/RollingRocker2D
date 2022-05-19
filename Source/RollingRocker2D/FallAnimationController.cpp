// Fill out your copyright notice in the Description page of Project Settings.


#include "FallAnimationController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFallAnimationController::UFallAnimationController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFallAnimationController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFallAnimationController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_IsAnimating)
	{
		return;
	}

	if (!ScalingCurve->IsValidLowLevelFast())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("FallAnimationController's ScalingCurve is not valid."));
		return;
	}

	if (m_ScaleTimer <= ScaleToZeroTime)
	{
		m_ScaleTimer += DeltaTime;

		// Location
		m_Velocity += FVector::DownVector * DeltaTime * m_Gravity;
		m_TargetSceneComponent->SetWorldLocation(m_TargetSceneComponent->GetComponentLocation() + m_Velocity * DeltaTime);

		// Rotation
		m_TargetSceneComponent->AddLocalRotation(m_RotationDeltaPerFrame);

		// Scale
		float scaledTime = FMath::Clamp(m_ScaleTimer / ScaleToZeroTime, 0, 1);
		float scale = ScalingCurve->GetFloatValue(scaledTime);
		m_TargetSceneComponent->SetWorldScale3D(FVector::One() * scale);

		if (m_ScaleTimer > ScaleToZeroTime)
		{
			m_IsAnimating = false;
			OnAnimationEnd.Broadcast(m_TargetSceneComponent);
		}
	}
}

void UFallAnimationController::PlayFallAnimationOnSceneComponent(USceneComponent* sceneComponent, FVector initialVelocity, FRotator rotationDeltaPerFrame, float gravity)
{
	m_IsAnimating = true;
	m_TargetSceneComponent = sceneComponent;
	m_Velocity = initialVelocity;
	m_RotationDeltaPerFrame = rotationDeltaPerFrame;
	m_Gravity = gravity;

	m_ScaleTimer = 0;

	if (OnFallSound->IsValidLowLevelFast())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OnFallSound);
	}

	OnAnimationBegin.Broadcast(sceneComponent);
}

void UFallAnimationController::StopFallAnimation()
{
	m_IsAnimating = false;
	OnAnimationEnd.Broadcast(m_TargetSceneComponent);
	m_TargetSceneComponent = nullptr;
}