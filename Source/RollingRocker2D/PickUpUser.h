// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpUser.generated.h"

USTRUCT(BlueprintType)
struct FPickUpData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AItemPickup* PickUp = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPickUpUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROLLINGROCKER2D_API IPickUpUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UsePickUp(FPickUpData pickUpData);
};
