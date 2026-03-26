// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACharacterBase.h"
#include "BossEnemy.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API ABossEnemy : public AACharacterBase
{
	GENERATED_BODY()

public:
	ABossEnemy();

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)override;


	UPROPERTY(EditAnywhere, Category = "Appearance")
	TArray<FVector> SoundRingPositions;

	UPROPERTY(VisibleDefaultsOnly, Category = "SoundRing")
	TArray < TObjectPtr<ASoundRingNext>> Ring_Array;
};
