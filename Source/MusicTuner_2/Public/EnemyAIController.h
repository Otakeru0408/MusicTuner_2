// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	FName TargetName = "TargetActor";
protected:
	virtual void OnPossess(APawn* InPawn)override;

	void InitPlayerReference();

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BT_Enemy;
};
