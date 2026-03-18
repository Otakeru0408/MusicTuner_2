// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBall.h"
#include "SoundRing.generated.h"

UCLASS()
class MUSICTUNER_2_API ASoundRing : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundRing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// UE5の標準スタイル
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInst;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInstanceConstant> M_SoundRing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	int BPM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	int RingNum;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	int SoundRingRadius_Single;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	int SoundRingRadius_All;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	float RingSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Objects")
	TArray<FEnemyBall> enemyBalls;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
	TObjectPtr<UStaticMeshComponent> Disc;

private:
	float PassTime = 0;
	int RingCount = 0;
};
