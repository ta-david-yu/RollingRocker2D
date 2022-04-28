// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Rod.h"
#include "Rocker.h"
#include "FallAnimationController.h"
#include "CanFall.h"
#include "Killable.h"
#include "RollingRockerPawn.generated.h"

USTRUCT(BlueprintType)
struct FMoveForwardEventData
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveAmount;
};

USTRUCT(BlueprintType)
struct FFallDownEventData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FDeathEventData
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveForwardEvent, FMoveForwardEventData, moveForwardEventData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFallDownEvent, FFallDownEventData, fallDownEventData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathEvent, FDeathEventData, deathEventData);

UCLASS()
class ROLLINGROCKER2D_API ARollingRockerPawn : public APawn, public ICanFall, public IKillable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URocker* Rocker = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* Rod = nullptr;

	UPROPERTY(BlueprintAssignable)
	FMoveForwardEvent OnMoveForward;
	
	UPROPERTY(BlueprintAssignable)
	FFallDownEvent OnFallDown;
	
	UPROPERTY(BlueprintAssignable)
	FDeathEvent OnDied;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UFallAnimationController> m_FallAnimationController = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool m_IsDead = false;

private:
	FTimerHandle m_FallKillDelayedTimerHandle;

protected:
	// Sets default values for this pawn's properties
	ARollingRockerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FallDownTo();									
	virtual void FallDownTo_Implementation() override; 
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;
	virtual bool IsDead_Implementation() const override { return m_IsDead; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FKillResult Kill();
	virtual FKillResult Kill_Implementation() override;

private:
	UFUNCTION()
	void handleOnFallAnimationEnd(USceneComponent* targetSceneComponent);

	UFUNCTION()
	void handleOnRodLocationChanged(FRodLocationChangedEventData eventData);
};
