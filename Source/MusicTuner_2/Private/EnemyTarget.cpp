// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTarget.h"
#include "ACharacterBase.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

// Sets default values
AEnemyTarget::AEnemyTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	if (HitUnableMat) {
		Sphere->SetMaterial(0, HitUnableMat);
	}
	Sphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyTarget::BeginPlay()
{
	Super::BeginPlay();
	isFollowing = true;
}

// Called every frame
void AEnemyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFollowing) {
		//通常の移動処理
		FVector forward = Sphere->GetForwardVector();
		FVector NextPos = forward * Speed * DeltaTime + GetActorLocation();
		SetActorLocation(NextPos);
	}

	//プレイヤーに一定期間でダメージを与え続けるための変数
	if (DamageTime > 0) {
		DamageTime -= DeltaTime;
	}

	//UE_LOG(LogTemp, Display, TEXT("Distance : %f"), CulcDistanceToParent());

	//Parentから一定範囲内はダメージを与えられないようにするが、そこを超えたらダメージできるようにする
	if (CulcDistanceToParent() >= NoDamageRange && HitEnableMat) {
		Sphere->SetMaterial(0, HitEnableMat);
	}
	else {
		Sphere->SetMaterial(0, HitUnableMat);
	}

	//ボールが蹴られたとき敵の方へ飛んでいく処理
	if (bIsShooting) {
		ShootingAlpha += DeltaTime / ShootingPassTime;

		if (ShootingAlpha >= 1) {
			bIsShooting = false;
			SetActorLocation(FVector(0, 0, -1000.0f));
			return;
		}

		FVector CurveValue = VectorCurve->GetVectorValue(ShootingAlpha);
		FVector NowPos = FMath::Lerp(ShootingStartPoint, ShootingEndPoint, CurveValue.X);
		float Offset = CurveValue.Y * ShootingHeight;
		NowPos.Z += Offset;

		SetActorLocation(NowPos);
	}

}

void AEnemyTarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 親クラスの処理を呼ぶ
	Super::NotifyActorBeginOverlap(OtherActor);

	// 相手が有効かつ自分自身ではないことを確認
	if (OtherActor && (OtherActor != this))
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		if (player && DamageTime <= 0) {
			UGameplayStatics::ApplyDamage(
				player,
				DamageNum,
				Enemy_Owner->GetController(),
				this,
				UDamageType::StaticClass()
			);

			if (AttackSound)UGameplayStatics::PlaySound2D(GetWorld(), AttackSound);
			DamageTime = DamageMaxTime;
		}
	}
}

float AEnemyTarget::CulcDistanceToParent() {
	if (!GetOwner())return 0.0f;

	FVector VectorToParent = GetOwner()->GetActorLocation() - GetActorLocation();
	return VectorToParent.Length();
}

bool AEnemyTarget::DamageToEnemy(int32 DamageValue, AActor* DamageCauser, bool IsComboHit) {
	AMainCharacter* player = Cast<AMainCharacter>(DamageCauser);

	if (player) {
		UGameplayStatics::ApplyDamage(
			Enemy_Owner,                 // ダメージを受けるアクター（この敵自身）
			player->GetNowDamageNum(),   // ダメージ量
			player->GetController(), // ダメージを与えた人のコントローラー
			DamageCauser,         // ダメージを引き起こしたアクター
			UDamageType::StaticClass() // ダメージタイプ（基本はこれでOK）
		);

		//ballがenemyの方に飛んでいく処理
		ShootingStartPoint = GetActorLocation();
		ShootingEndPoint = Enemy_Owner->GetActorLocation();

		float Distance = (ShootingEndPoint - ShootingStartPoint).Length();

		//Start→Endで常におなじスピードで進んでほしい
		bIsShooting = true;
		ShootingAlpha = 0.0f;

		//Followをはずし一時的にEnemyから解放されるイメージ(SoundRingで中心に戻される際にtrueになる)
		isFollowing = false;

		return true;
	}

	return false;
}

bool AEnemyTarget::CanDamage() {
	if (CulcDistanceToParent() > NoDamageRange)return true;

	return false;
}

void AEnemyTarget::SetEnemyReference(AActor* enemy) {
	Enemy_Owner = Cast<AACharacterBase>(enemy);
}