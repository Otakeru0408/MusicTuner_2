// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController() {

}

void AEnemyAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	GetWorldTimerManager().SetTimerForNextTick(this, &AEnemyAIController::InitPlayerReference);
}

void AEnemyAIController::InitPlayerReference() {
	if (BT_Enemy) {
		if (RunBehaviorTree(BT_Enemy)) {	//BehaviorTreeを実行&初期化
			// Blackboardコンポーネントを取得
			UBlackboardComponent* BBComp = GetBlackboardComponent();
			if (BBComp)
			{
				// 例：プレイヤーキャラクターを検索してセットする場合
				AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

				// "TargetActor" という名前のキーに値をセット
				// FNameで指定するため、Blackboard上の名前と一致させる必要があります
				//BBComp->SetValueAsObject(TargetName, PlayerPawn);

			}
		}
	}
}

void AEnemyAIController::SetPatrolPoint(const TArray<AActor*> locations) {
	PatrolLocations = locations;
	if (PatrolLocations.Num() > 0) {
		NowPatrolIndex = 0;
	}
}

FVector AEnemyAIController::GetPatrolPoint() {
	if (PatrolLocations.Num() > 0)return PatrolLocations[NowPatrolIndex]->GetActorLocation();

	return GetPawn()->GetActorLocation();
}

void AEnemyAIController::PopPatrolPoint() {
	NowPatrolIndex = (NowPatrolIndex + 1) % PatrolLocations.Num();
}