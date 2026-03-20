// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckSensor.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API UBTS_CheckSensor : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckSensor();

protected:
	// サービスが実行されている間、定期的に呼ばれるメイン処理
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Blackboardのキーを指定するための変数（エディタで選択可能にする）
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
