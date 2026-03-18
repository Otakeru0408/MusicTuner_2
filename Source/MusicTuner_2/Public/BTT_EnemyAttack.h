// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_EnemyAttack.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API UBTT_EnemyAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_EnemyAttack();

	// タスク実行時に呼ばれるメイン関数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	// Blackboardのキーを指定するための変数（エディタでセットする）
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
};
