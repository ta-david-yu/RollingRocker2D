// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Killable.generated.h"

USTRUCT(BlueprintType)
struct FKillParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayDefaultDeathAnimation = false;
};

USTRUCT(BlueprintType)
struct FKillResult
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Success = true;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKillable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROLLINGROCKER2D_API IKillable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FKillResult Kill(FKillParameters killParameters);
};
