// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundRingNext.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" // 必要に応じて
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASoundRingNext::ASoundRingNext()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	RootComponent = Disc;
	Disc->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void ASoundRingNext::BeginPlay()
{
	Super::BeginPlay();

	//RingNum→RingCountの変換
	//実際には3という数字で2本になり5で3本になるので、実際に欲しい本数(RingNum)から提出する数値(RingCount)に変換する
	RingCount = RingNum * 2 - 1;

	//現在のDiscのScale,RingCountから2つの波のあいだの距離を計算する
	//50はDiscのScale=1の時の半径のサイズ。DiscのScaleはどれも一緒なのでXを使用
	SoundRingRadius_Single = (50.0 / RingCount) * Disc->GetRelativeScale3D().X;
	SoundRingRadius_All = 50 * Disc->GetRelativeScale3D().X;

	//Ringの広がるスピードを計算する
	//RingNumがなんぼでもBPM=60なら1秒に一回外側にRingが来る
	RingSpeed = (BPM / 60.0) * SoundRingRadius_Single * 2;

	if (M_SoundRing) {
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(M_SoundRing.Get(), this);
		DynamicMaterialInst->SetScalarParameterValue("TimeValue", 0);
		DynamicMaterialInst->SetScalarParameterValue("BPM", BPM);
		DynamicMaterialInst->SetScalarParameterValue("Size", RingCount);

		Disc->SetMaterial(0, DynamicMaterialInst);
	}

	//SoundRingをPlayerの方へ向ける
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector PlayerPos(0.0f, 0.0f, 0.0f);
	if (PlayerPawn)
	{
		PlayerPos = PlayerPawn->GetActorLocation();
	}

	FVector Start = GetActorLocation(); // 自分の位置
	FVector Target = PlayerPos;    // 相手（プレイヤー）の位置

	// 回転値を計算
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	// Actorに適用する（相手の方を向かせる）
	SetActorRotation(LookAtRotation);

	//EmemyBallを生成する
	for (int i = 0; i < RingNum; i++) {
		//SpawnActorFromClassは思ったよりもめんどくさそう
		GenerateEnemyTarget(10 * i);
	}
}

// Called every frame
void ASoundRingNext::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PassTime += DeltaTime;
	DynamicMaterialInst->SetScalarParameterValue("TimeValue", PassTime);
}

void ASoundRingNext::GenerateEnemyTarget(float deg) {
	// 1. スポーンする位置と回転を設定
	FVector UpVec = GetActorUpVector();

	//仮にPlayerの位置を原点とする
	// 0番目のプレイヤーポーン（操作キャラクター）を取得
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector PlayerPos(0.0f, 0.0f, 0.0f);
	if (PlayerPawn)
	{
		// プレイヤーの現在位置（FVector）を取得
		PlayerPos = PlayerPawn->GetActorLocation();
	}

	FVector VecPlayerToSelf = GetActorLocation() - PlayerPos;
	VecPlayerToSelf.Normalize();

	//Locationの設定を行う
	FVector Location = RotateInAxis(UpVec, VecPlayerToSelf, deg) * 500 + GetActorLocation();
	//Location *= 500;
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;

	// 2. 誰がスポーンさせたか（Owner）などの詳細設定（任意）
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 3. UWorldを取得してスポーン実行
	UWorld* World = GetWorld();
	if (World && ActorClassToSpawn) // ActorClassToSpawnは TSubclassOf<AActor> 型の変数
	{
		AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClassToSpawn, Location, Rotation, SpawnParams);

		if (SpawnedActor)
		{
			// 生成成功後の処理（例：初期化関数の呼び出しなど）
			UE_LOG(LogTemp, Log, TEXT("Actor Spawned: %s"), *SpawnedActor->GetName());
		}
	}
}

FVector ASoundRingNext::RotateInAxis(FVector AxisVec, FVector BaseVec, float Deg) {
	// 2. 回転の軸となるベクトル (UpVector)
	// ※回転軸にする場合は必ず正規化（Normalize）する必要があります
	FVector Axis = AxisVec.GetSafeNormal();

	// 3. 回転させたい角度（例：45度）
	float AngleDegrees = Deg;
	float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

	// 4. 軸と角度からクォータニオンを作成
	FQuat RotationQuat = FQuat(Axis, AngleRadians);

	// 5. ベクトルを回転させる
	FVector RotatedVector = RotationQuat.RotateVector(BaseVec);
	return RotatedVector;
}

void ASoundRingNext::SetEnemyTargetPos() {

}