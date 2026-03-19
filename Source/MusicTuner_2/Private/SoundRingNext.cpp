// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundRingNext.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" // 必要に応じて
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBall.h"
#include "EnemyTarget.h"
#include "Components/AudioComponent.h"

// Sets default values
ASoundRingNext::ASoundRingNext()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	RootComponent = Disc;
	Disc->SetCollisionProfileName(FName("NoCollision"));

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	Audio->SetupAttachment(RootComponent);
	Audio->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ASoundRingNext::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

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

	//SoundRingをPlayerに向ける
	SetSoundRingRotation();

	//EmemyBallを生成する
	//1度に何個Ballを生成するか
	float DividedAngle = 360.0f / RingDivideNum;

	for (int i = 0; i < RingNum - 1; i++) {
		FEnemyBall BallArray;
		for (int j = 0; j < RingDivideNum; j++) {
			AActor* target = GenerateEnemyTarget(DividedAngle * j, SoundRingRadius_Single * (i * 2 + 1));
			if (target)BallArray.Balls.Add(target);
		}
		enemyBalls.Add(BallArray);
	}

	//BPMで弾を中心にリセットする機能
	// タイマーの開始
	if (BPM > 0) {
		GetWorldTimerManager().SetTimer(
			ResetTimerHandle,             // ハンドル
			this,                      // 実行するオブジェクト
			&ASoundRingNext::EventOnBPM, // 実行する関数のアドレス
			60.0f / BPM,                      // 時間（秒）
			true                       // ループするかどうか（falseなら1回切り）
		);
	}

}

//BPMにあわせて実行される関数をまとめているもの
void ASoundRingNext::EventOnBPM() {
	ResetBallsPosition();
	Audio->SetVolumeMultiplier(CulcAttenuationRate());
	Audio->Play();
}

// Called every frame
void ASoundRingNext::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PassTime += DeltaTime;
	DynamicMaterialInst->SetScalarParameterValue("TimeValue", PassTime);

	SetEnemyTargetPos();
}

AActor* ASoundRingNext::GenerateEnemyTarget(float deg, float length) {
	// 1. スポーンする位置と回転を設定
	FVector UpVec = GetActorUpVector();

	FVector PlayerPos(0.0f, 0.0f, 0.0f);

	if (!IsValid(PlayerPawn))
	{
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	// プレイヤーの現在位置（FVector）を取得
	PlayerPos = PlayerPawn->GetActorLocation();

	FVector VecPlayerToSelf = GetActorLocation() - PlayerPos;
	VecPlayerToSelf.Normalize();

	//Locationの設定を行う
	FVector Location = RotateInAxis(UpVec, VecPlayerToSelf, deg) * length + GetActorLocation();
	//算出したLocationを元に、中心から各ballまでのベクトルを取得→Rotationにする
	FVector Direction = Location - GetActorLocation();
	FRotator Rotation = Direction.Rotation();
	FTransform ballTrans(Rotation, Location, FVector(0.5f));

	// 2. 誰がスポーンさせたか（Owner）などの詳細設定（任意）
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 3. UWorldを取得してスポーン実行
	UWorld* World = GetWorld();
	if (World && ActorClassToSpawn) // ActorClassToSpawnは TSubclassOf<AActor> 型の変数
	{
		AEnemyTarget* SpawnedActor = World->SpawnActorDeferred<AEnemyTarget>(
			ActorClassToSpawn, ballTrans,
			SpawnParams.Owner, SpawnParams.Instigator, SpawnParams.SpawnCollisionHandlingOverride);

		if (SpawnedActor)
		{
			// 生成成功後の処理（例：初期化関数の呼び出しなど）
			SpawnedActor->SetSpeed(SoundRingRadius_Single * 2 * BPM / 60.0f);
			SpawnedActor->SetEnemyReference(GetOwner());
			return SpawnedActor;
		}
	}

	return nullptr;
}

//AxisVecを軸として、BaseVecをDeg°だけ回したときの位置を計算する
FVector ASoundRingNext::RotateInAxis(FVector AxisVec, FVector BaseVec, float Deg) {
	// 2. 回転の軸となるベクトル (UpVector)
	// ※回転軸にする場合は必ず正規化（Normalize）する必要があります
	FVector Axis = AxisVec.GetSafeNormal();

	// 3. 回転させたい角度（例：45度）
	float AngleRadians = FMath::DegreesToRadians(Deg);

	// 4. 軸と角度からクォータニオンを作成
	FQuat RotationQuat = FQuat(Axis, AngleRadians);

	// 5. ベクトルを回転させる
	FVector RotatedVector = RotationQuat.RotateVector(BaseVec);
	return RotatedVector;
}

void ASoundRingNext::SetSoundRingRotation() {
	//SoundRingをPlayerの方へ向ける
	FVector PlayerPos(0.0f, 0.0f, 0.0f);

	if (!IsValid(PlayerPawn)) {
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	PlayerPos = PlayerPawn->GetActorLocation();

	// 回転値を計算
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), PlayerPos);

	// Actorに適用する（相手の方を向かせる）
	SetActorRotation(LookAtRotation);
}

void ASoundRingNext::SetEnemyTargetPos() {
	FVector nowSoundRingPos = GetActorLocation();
	//移動中はSoundRingとTargetの動きにばらつきが発生するので移動量も勘案に入れる
	FVector RingForward = GetOwner()->GetActorForwardVector() * (GetOwner()->GetVelocity().Length() / 100.0f);

	//1.基本ベクトルの計算
	if (!IsValid(PlayerPawn)) {
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	FVector BaseVec = nowSoundRingPos - PlayerPawn->GetActorLocation();
	BaseVec.Normalize();

	//内積だけでは-の角度が計算できないのでSoundRingのUpVecを取得しておく
	FVector BaseUpVec = GetActorUpVector();

	//2.軸ベクトルの計算
	SetSoundRingRotation();
	FVector AxisVec = GetActorUpVector();

	bool isChecked = false;		//各Ringの半径をしらべる（各Ringにつき1個のBallだけしらべる）

	for (FEnemyBall& enemyball : enemyBalls) {
		isChecked = false;
		for (AActor* balls : enemyball.Balls) {
			//3.基本ベクトルと、(中心→Ball)のベクトルがなす角を計算する
			FVector BallVec = balls->GetActorLocation() - nowSoundRingPos;
			float Length = BallVec.Length();
			BallVec.Normalize();

			//2つのベクトルの内積から角度を求める
			float DotProductValue = FVector::DotProduct(BaseVec, BallVec);
			float BallDegree = FMath::RadiansToDegrees(FMath::Acos(DotProductValue));

			//外積もとって-かどうか確かめる
			FVector CrossProduct = FVector::CrossProduct(BaseVec, BallVec);
			if (FVector::DotProduct(CrossProduct, BaseUpVec) < 0) {
				BallDegree *= -1;
			}

			//4.Ballの正しい位置を決定する
			balls->SetActorLocation(nowSoundRingPos + Length * RotateInAxis(AxisVec, BaseVec, BallDegree) + RingForward * 2);

			//1周分のBallを保持するenemyBallに、その周の半径を持たせる
			if (!isChecked) {
				enemyball.NowRadius = Length;
				isChecked = true;
			}
		}
	}
}

//BPMごとに呼び出される、ボールを中心に戻す関数
void ASoundRingNext::ResetBallsPosition() {
	FVector nowPos = GetActorLocation();
	for (AActor* ball : enemyBalls[resetBallIndex].Balls) {
		FVector direction = ball->GetActorLocation() - nowPos;
		direction.Normalize();
		ball->SetActorLocation(nowPos + direction * SoundRingRadius_Single);

	}

	resetBallIndex = (resetBallIndex + 1) % enemyBalls.Num();
}

//Audioの音量を距離によって変化させるための割合を計測する
float ASoundRingNext::CulcAttenuationRate() {
	FVector PlayerPos(0.0f, 0.0f, 0.0f);

	if (!IsValid(PlayerPawn)) {
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	PlayerPos = PlayerPawn->GetActorLocation();

	//Playerと自身の距離を測る
	float Distance = (PlayerPos - GetActorLocation()).Length();

	//AttenuationRangeよりもはなれていたら0、一番近かったら1としたい。
	float Rate = Distance / AttenuationRange;
	Rate = 1 - Rate;

	return FMath::Clamp(Rate, 0.0f, 1.0f);
}

void ASoundRingNext::DestroyMySelf() {
	UE_LOG(LogTemp, Log, TEXT("Destroy MySelf"));
	for (int i = enemyBalls.Num() - 1; i >= 0; i--) {
		TArray<TObjectPtr<AActor>>& balls = enemyBalls[i].Balls;
		for (int j = enemyBalls[i].Balls.Num() - 1; j >= 0; j--) {
			UE_LOG(LogTemp, Log, TEXT("Destroy : %s"), *balls[j]->GetName());
			balls[j]->Destroy();
		}
	}
	Destroy();
}

void ASoundRingNext::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Log, TEXT("SoundRing Endplay"));
}