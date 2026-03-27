// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundRingNext.generated.h"

struct FEnemyBall;
class AEnemyTarget;
class UAudioComponent;

UCLASS()
class MUSICTUNER_2_API ASoundRingNext : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundRingNext();

	UFUNCTION()
	void SetBPM(int num) { BPM = num; }

	UFUNCTION()
	void SetRingDivideNum(int num) { RingDivideNum = num; }

	UFUNCTION()
	void SetRingNum(int num) { RingNum = num; }

	UFUNCTION()
	void DestroyMySelf();

	UPROPERTY()
	TObjectPtr<UMaterialInterface> HitEnableMat;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//EnemyTargetを生成する関数
	UFUNCTION(BlueprintCallable)
	AActor* GenerateEnemyTarget(float deg, float length);

	//毎フレームPlayerの方向へ向けてSoundRingと弾の位置角度を調整する関数
	UFUNCTION(BlueprintCallable)
	void SetEnemyTargetPos();

	//SoundRingの水平面をPlayerに向ける関数
	UFUNCTION(BlueprintCallable)
	void SetSoundRingRotation();

	//BallたちがSoundRingを循環して動くようにする関数
	UFUNCTION(BlueprintCallable)
	void ResetBallsPosition();

	UFUNCTION()
	void EventOnBPM();

	UFUNCTION()
	float CulcAttenuationRate();

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr <UAudioComponent> Audio;

	UPROPERTY(VisibleDefaultsOnly, Category = "Timer")
	int resetBallIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float AttenuationRange = 1400.0f;

	//あるベクトルを中心としてベースとなるベクトルを回転させたときのベクトルを計算する
	FVector RotateInAxis(FVector AxisVec, FVector BaseVec, float Deg);

	// UE5の標準スタイル
	UPROPERTY(VisibleAnywhere, Category = "Materials")
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInst;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TObjectPtr<class UMaterialInstanceConstant> M_SoundRing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	int BPM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	int RingDivideNum;

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
	TSubclassOf<AEnemyTarget> ActorClassToSpawn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APawn> PlayerPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = "Timer")
	FTimerHandle ResetTimerHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "StaticMesh")
	TObjectPtr<UStaticMeshComponent> Disc;

private:
	float PassTime = 0;
	int RingCount = 0;
};
