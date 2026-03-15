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
}

// Called when the game starts or when spawned
void AEnemyTarget::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forward = Sphere->GetForwardVector();
	FVector NextPos = forward * Speed * DeltaTime + GetActorLocation();
	SetActorLocation(NextPos);

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
			GetOwner(),                 // ダメージを受けるアクター（この敵自身）
			(float)DamageValue,   // ダメージ量
			player->GetController(), // ダメージを与えた人のコントローラー
			DamageCauser,         // ダメージを引き起こしたアクター
			UDamageType::StaticClass() // ダメージタイプ（基本はこれでOK）
		);

		// ログ出力（デバッグ用）
		UE_LOG(LogTemp, Log, TEXT("%s took damage from CharacterBase!"), *GetName());

		//攻撃時敵がPlayerに気づいていないなら気づかせる
		/*if (Enemy_Owner) {
			AEnemyAIController* AIC_Enemy = Cast<AEnemyAIController>(Enemy_Owner->GetController());
			if (AIC_Enemy) {
				UBlackboardComponent* BBComp = AIC_Enemy->GetBlackboardComponent();
				if (BBComp)
				{
					BBComp->SetValueAsObject(AIC_Enemy->TargetName, player);
				}
			}
		}*/

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