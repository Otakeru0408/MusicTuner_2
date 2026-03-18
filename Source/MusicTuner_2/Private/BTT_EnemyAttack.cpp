// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "ACharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_EnemyAttack::UBTT_EnemyAttack() {
	NodeName = "EnemyAttack";
}

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) EBTNodeResult::Failed;

	AEnemyAIController* EnemyAIC = Cast<AEnemyAIController>(AIC);

	AACharacterBase* MyPawn = Cast<AACharacterBase>(EnemyAIC->GetPawn());

	if (EnemyAIC && MyBlackboard && MyPawn) {
		//Ç‹Ç∏çUåÇÇÃëOÇ…ìGÇÃï˚ÇÇµÇ¡Ç©ÇËå¸Ç≠
		AActor* Target = Cast<AActor>(MyBlackboard->GetValueAsObject(TargetKey.SelectedKeyName));
		FRotator VecToPlayer = UKismetMathLibrary::FindLookAtRotation(
			MyPawn->GetActorLocation(),
			Target->GetActorLocation()
		);
		VecToPlayer.Pitch = 0;
		VecToPlayer.Roll = 0;

		MyPawn->SetActorRotation(VecToPlayer);

		//çUåÇÉÇÅ[ÉVÉáÉìçƒê∂
		MyPawn->StartAttackAnim();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
