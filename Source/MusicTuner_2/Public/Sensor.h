// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sensor.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MUSICTUNER_2_API USensor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USensor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float TraceLength = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float TraceAngle = 10.0f;

	UPROPERTY()
	float BoxWidth = 0.0f;

	UPROPERTY()
	TObjectPtr<AActor> Owner;

public:

	bool CheckSensor(TArray<TObjectPtr<AActor>>& OutResults);

};
