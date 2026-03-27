// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "SoundRingNext.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "PlayerHP.h"
#include "RewardCrystal.h"
#include "Sensor.h"
#include "DamagePopup.h"

ABossEnemy::ABossEnemy() {

}

void ABossEnemy::BeginPlay() {
	ACharacter::BeginPlay();
	//Super::BeginPlay();	親クラスのBeginPlayは実行しない

	for (int i = 0; i < SoundRingPositions.Num(); i++) {

		//SoundRingを生成する
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform SRTrans(GetActorRotation(), GetActorLocation() + SoundRingPositions[i] + SoundRingOffset, FVector(1.0f));

		UWorld* world = GetWorld();
		if (world && SoundRingClass) {
			SoundRing = world->SpawnActorDeferred<ASoundRingNext>(
				SoundRingClass, SRTrans,
				SpawnParams.Owner, SpawnParams.Instigator, SpawnParams.SpawnCollisionHandlingOverride);

			if (SoundRing) {
				SoundRing->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
				SoundRing->SetBPM(BPM);
				SoundRing->SetRingDivideNum(RingDividedNum);
				SoundRing->SetRingNum(RingNum);
				if (HitEnableMat)SoundRing->HitEnableMat = HitEnableMat;
			}
		}

		Ring_Array.Add(SoundRing);
	}

	if (SkinMat) {
		GetMesh()->SetMaterial(0, SkinMat);
	}

	//WidgetComponentにBPのWidgetを割り当てる
	if (Widget_HP_Class) {
		HPWidget->SetWidgetClass(Widget_HP_Class);
		HPWidget->InitWidget();	//Widgetのインスタンスを作成する

		WidgetData = Cast<UPlayerHP>(HPWidget->GetUserWidgetObject());
		WidgetData->InitData(Health->GetMaxHP());
	}


	//後のためにAIControllerとBlackboardを保持しておく
	AIC_Enemy = Cast <AEnemyAIController>(GetController());
	BB_Enemy = AIC_Enemy->GetBlackboardComponent();

	// 自分の OnTakeAnyDamage イベントに、作成した関数を登録する
	OnTakeAnyDamage.AddDynamic(this, &ABossEnemy::HandleTakeAnyDamage);

	//AIControllerにPatrolPointを渡す
	AIC_Enemy->SetPatrolPoint(PatrolLocations);

	//Playerが死亡したらTargetを外す処理をバインドしておく
	if (AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		player->OnPlayerDied.AddDynamic(this, &ABossEnemy::UnBindAITarget);
	}
}

void ABossEnemy::Tick(float DeltaTime) {
	ACharacter::Tick(DeltaTime);

	if (!BB_Enemy && AIC_Enemy)BB_Enemy = AIC_Enemy->GetBlackboardComponent();

	//常にSoundRingを自身との相対位置に置く
	for (int i = 0; i < Ring_Array.Num(); i++) {
		Ring_Array[i]->SetActorLocation(GetActorLocation() + SoundRingPositions[i]);
	}

}

void ABossEnemy::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	AACharacterBase::HandleTakeAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	if (!Health->GetIsAlive()) {
		for (int i = Ring_Array.Num() - 1; i >= 0; i--) {
			Ring_Array[i]->DestroyMySelf();
		}
		Ring_Array.Empty();
	}
}
