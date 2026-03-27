// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoundTuner_Instance.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API USoundTuner_Instance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Health")
	float PlayerHP = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Crystal")
	int crystalNum = 0;
};
