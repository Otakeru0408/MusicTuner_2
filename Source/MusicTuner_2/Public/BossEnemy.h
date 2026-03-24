// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACharacterBase.h"
#include "BossEnemy.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API ABossEnemy : public AACharacterBase
{
	GENERATED_BODY()

public:
	ABossEnemy();

protected:
	virtual void BeginPlay()override;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	TArray<FVector> SoundRingPositions;

	UPROPERTY(VisibleDefaultsOnly, Category = "SoundRing")
	TArray < TObjectPtr<ASoundRingNext>> Ring_Array;
};
