// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBall.generated.h"


USTRUCT(BlueprintType)
struct FEnemyBall
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AActor>> Balls;
	float NowRadius = 0.0f;
};
