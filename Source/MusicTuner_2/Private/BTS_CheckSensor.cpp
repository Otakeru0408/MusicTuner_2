// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckSensor.h"
#include "ACharacterBase.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CheckSensor::UBTS_CheckSensor() {

}

void UBTS_CheckSensor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	AEnemyAIController* AIC = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIC) return;

	AACharacterBase* enemy = Cast<AACharacterBase>(AIC->GetPawn());
	if (!enemy)return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	if (BBComp) {
		AActor* result = enemy->CheckSensor();

		//if (result)UE_LOG(LogTemp, Log, TEXT("Results exists"));
		/*if (result) {
			BBComp->SetValueAsObject(TargetActorKey.SelectedKeyName, result);
		}*/
	}
}