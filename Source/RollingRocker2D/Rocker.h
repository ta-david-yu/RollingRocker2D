// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Rod.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Rocker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URocker : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	URod* m_Rod;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_LocationOnRod = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_OnRodGravity = 9.8f;

public:	
	// Sets default values for this component's properties
	URocker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
