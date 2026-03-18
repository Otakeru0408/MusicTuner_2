// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_PopPatrolPoint.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API UBTT_PopPatrolPoint : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_PopPatrolPoint();

	// タスク実行時に呼ばれるメイン関数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
