// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "BossEnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API ABossEnemyAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	ABossEnemyAIController();

protected:
	virtual void InitPlayerReference()override;
	virtual void Tick(float DeltaTime)override;
};
