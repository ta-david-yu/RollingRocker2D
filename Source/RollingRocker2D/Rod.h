// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Rod.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URod : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_MinAngle = -45;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_MaxAngle = 45;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_Length = 200;

public:
	// Sets default values for this component's properties
	URod();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
