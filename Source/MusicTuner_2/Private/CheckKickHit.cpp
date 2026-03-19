// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckKickHit.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"	
#include "DrawDebugHelpers.h"
#include "DamageTarget.h"
#include "MainCharacter.h"
#include "Sound/SoundBase.h"

void UCheckKickHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	HitActors.Reset();
	/*AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());
	player->DamageCount = 0;*/
}
void UCheckKickHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())return;

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());
	FVector SocketLocation = MeshComp->GetSocketLocation(HitPartName);

	//ObjectChannel : EnemyBall → ECC_GameTraceChannel1
	//				: Enemy     → ECC_GameTraceChannel2
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); //Enemyball
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2)); //Enemy

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(MeshComp->GetOwner()); // 自分自身は無視

	TArray<AActor*> HitObjects;

	// 3. 球体判定の実行
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		MeshComp->GetWorld(),
		SocketLocation,
		SphereRadius,
		ObjectTypes,
		nullptr, // 特定のクラスで絞り込む場合はここに指定（例: AEnemy::StaticClass()）
		IgnoreActors,
		HitObjects
	);

	//当たり判定を表示する
	DrawDebugSphere(
		MeshComp->GetWorld(),
		SocketLocation,
		SphereRadius,
		12,             // 球体のセグメント数（多いほど滑らかな球になります）
		FColor::Blue,    // 色
		false,          // 永続的に表示するか（falseなら1フレームで消える）
		-1.0f,          // 表示時間（上の引数がfalseの場合、-1なら1フレーム分）
		0,              // 描画の優先順位
		1.0f            // 線の太さ
	);

	if (bHit) {
		//ヒットしたものをすべて調べる
		for (AActor* actor : HitObjects) {
			if (HitActors.Contains(actor))continue;	//今回の攻撃ですでにヒットしているものには攻撃しない

			if (IDamageTarget* target = Cast<IDamageTarget>(actor)) {
				if (!target->CanDamage()) continue;	//相手にダメージを与えられるかチェック

				player->DamageCount++;	//この１撃でいくつのActorにヒットしたか計算
				bool isNotThirdAfterHit = player->CheckHitCount();	//この攻撃でコンボがつながるか(未実装)

				//相手に攻撃する
				bool result = target->DamageToEnemy(player->GetDamageNum(), MeshComp->GetOwner(), true);
				if (result) {
					//UE_LOG(LogTemp, Log, TEXT("Attack Success"));

					//攻撃hit時にカメラシェイク
					player->StartCameraShake();
					//打撃音も鳴らす
					UGameplayStatics::PlaySound2D(MeshComp->GetWorld(), DamageSound);

					//3コンボ目より後の攻撃はダメージが通るだけで音を鳴らさない。
					if (isNotThirdAfterHit) {
						player->PlayHitSound();
					}
				}
			}
			HitActors.Add(actor);
		}
	}
	else {
		//Hitしたものがなければ終了
		return;
	}
}
void UCheckKickHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}