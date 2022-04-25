// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Rod.h"
#include "RodAnimationController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API URodAnimationController : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* m_Rod = nullptr;

public:
	// Sets default values for this component's properties
	URodAnimationController();
	
	UFUNCTION(BlueprintCallable)
	void SetRod(URod* rod);

private:
	UFUNCTION()
	void handleOnRodLocationChanged(FRodLocationChangedEventData eventData);
};
