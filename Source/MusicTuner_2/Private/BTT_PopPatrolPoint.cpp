// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PopPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

UBTT_PopPatrolPoint::UBTT_PopPatrolPoint() {
	NodeName = "PopPatrolPoint"; // BTエディタ上に表示される名前
}

EBTNodeResult::Type UBTT_PopPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) EBTNodeResult::Failed;

	AEnemyAIController* EnemyAIC = Cast<AEnemyAIController>(AIC);
	if (EnemyAIC) {
		EnemyAIC->PopPatrolPoint();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}