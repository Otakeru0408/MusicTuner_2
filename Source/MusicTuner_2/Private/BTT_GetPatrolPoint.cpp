// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

UBTT_GetPatrolPoint::UBTT_GetPatrolPoint() {
	NodeName = "GetPatrolPoint"; // BTエディタ上に表示される名前
}

EBTNodeResult::Type UBTT_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) EBTNodeResult::Failed;

	AEnemyAIController* EnemyAIC = Cast<AEnemyAIController>(AIC);
	if (EnemyAIC && MyBlackboard) {
		MyBlackboard->SetValueAsVector(TargetKey.SelectedKeyName, EnemyAIC->GetPatrolPoint());

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}