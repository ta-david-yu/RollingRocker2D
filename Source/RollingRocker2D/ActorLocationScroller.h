// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorLocationScroller.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UActorLocationScroller : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float m_InitialSpeed = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1.0", UIMin = "0", UIMax = "1.0"))
	float m_SpeedMultiplier = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool m_DoSweep = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float m_CurrentSpeed = 1.0f;

public:	
	// Sets default values for this component's properties
	UActorLocationScroller();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
