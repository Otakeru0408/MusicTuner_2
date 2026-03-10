// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBall.h"
#include "EnemyTarget.h"
#include "SoundRingNext.generated.h"

UCLASS()
class MUSICTUNER_2_API ASoundRingNext : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundRingNext();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//EnemyTargetを生成する関数
	void GenerateEnemyTarget(float deg);

	void SetEnemyTargetPos();

	//あるベクトルを中心としてベースとなるベクトルを回転させたときのベクトルを計算する
	FVector RotateInAxis(FVector AxisVec, FVector BaseVec, float Deg);

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

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> ActorClassToSpawn;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
	TObjectPtr<UStaticMeshComponent> Disc;

private:
	float PassTime = 0;
	int RingCount = 0;
};
