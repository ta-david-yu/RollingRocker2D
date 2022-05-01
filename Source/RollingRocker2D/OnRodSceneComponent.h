// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rod.h"
#include "Components/SceneComponent.h"
#include "OnRodSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROLLINGROCKER2D_API UOnRodSceneComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool m_IsTransformAppliedOnActor = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_Radius = 0.5f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URod* m_Rod = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float m_LocationOnRod = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator m_LastRotationDelta;

public:	
	// Sets default values for this component's properties
	UOnRodSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetRod(URod* rod);

	UFUNCTION(BlueprintCallable)
	float GetLocationOnRod() const { return m_LocationOnRod; }

	UFUNCTION(BlueprintCallable)
	void SetLocationOnRod(float locationOnRod) { m_LocationOnRod = locationOnRod; }
	
	UFUNCTION(BlueprintCallable)
	FRotator GetAngularVelocity(float locationDelta) const;
	
	UFUNCTION(BlueprintCallable)
	float InstantMoveClamp(float locationDelta);
	
	UFUNCTION(BlueprintCallable)
	void RotateWithLocationDelta(float locationDelta);

	UFUNCTION(BlueprintCallable)
	void SnapWorldLocationToRod();

private:
	UFUNCTION()
	void handleOnRodLocationChanged(FRodLocationChangedEventData eventData);

};
