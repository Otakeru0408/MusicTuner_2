// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossEnemyAIController::ABossEnemyAIController() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABossEnemyAIController::InitPlayerReference() {
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
				BBComp->SetValueAsObject(TargetName, PlayerPawn);

			}
		}
	}
}

void ABossEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}