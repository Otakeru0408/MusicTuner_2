// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacterBase.h"
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

// Sets default values
//コンストラクタ
AACharacterBase::AACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// コンポーネントの生成
	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComponent"));

	// メッシュやルートにアタッチ（頭上に配置したい場合は Mesh にアタッチするのが一般的）
	HPWidget->SetupAttachment(GetMesh());

	HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidget->SetDrawAtDesiredSize(true);

	Sensor = CreateDefaultSubobject<USensor>(TEXT("AC_Sensor"));
}

// Called when the game starts or when spawned
void AACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//SoundRingを生成する
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform SRTrans(GetActorRotation(), GetActorLocation() + SoundRingOffset, FVector(1.0f));

	UWorld* world = GetWorld();
	if (SoundRingClass) UE_LOG(LogTemp, Display, TEXT("Instantiate"));
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
	if (SkinMat) {
		GetMesh()->SetMaterial(0, SkinMat);
	}

	//WidgetComponentにBPのWidgetを割り当てる
	if (Widget_HP_Class) {
		HPWidget->SetWidgetClass(Widget_HP_Class);
		WidgetData = Cast<UPlayerHP>(HPWidget->GetUserWidgetObject());
		WidgetData->InitData(Health->GetMaxHP());
	}

	//後のためにAIControllerとBlackboardを保持しておく
	AIC_Enemy = Cast <AEnemyAIController>(GetController());
	BB_Enemy = AIC_Enemy->GetBlackboardComponent();

	// 自分の OnTakeAnyDamage イベントに、作成した関数を登録する
	OnTakeAnyDamage.AddDynamic(this, &AACharacterBase::HandleTakeAnyDamage);

	//AIControllerにPatrolPointを渡す
	AIC_Enemy->SetPatrolPoint(PatrolLocations);

	//Playerが死亡したらTargetを外す処理をバインドしておく
	if (AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		player->OnPlayerDied.AddDynamic(this, &AACharacterBase::UnBindAITarget);
	}

}

// Called every frame
void AACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!BB_Enemy)BB_Enemy = AIC_Enemy->GetBlackboardComponent();
}

//ダメージを受けた時の関数
void AACharacterBase::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	//UE_LOG(LogTemp, Log, TEXT("Damaged!"));

	//HPを更新する
	Health->UpdateHP(Damage);

	//HPのUIを更新する
	if (WidgetData)WidgetData->UpdateHP(Health->GetCurrentPercent(), Health->GetCurrectHP());

	//ダメージのUIを表示する
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform UITrans(GetActorRotation(), GetActorLocation() + FVector(0, 0, DamageTextOffset_Z), GetActorScale());

	AMainCharacter* player = Cast<AMainCharacter>(DamageCauser);

	//攻撃を受けた時にそのダメージ数を表示
	if (DamagePopupUI) {
		ADamagePopup* popup = GetWorld()->SpawnActor<ADamagePopup>(
			DamagePopupUI, UITrans, SpawnParams
		);
		if (popup) {
			UE_LOG(LogTemp, Log, TEXT("Generate Popup"));
			popup->StartAnimation(Damage, player->ComboCount);
		}
	}

	//Combo3撃目はノックバックする
	if (player->ComboCount == player->MaxComboCount) {
		RequestStateMontage(EEnemyState::Defending, Defend_Montage);
		ApplyKnockBack(player->GetActorLocation(), player->GetKnockBackPower());
	}

	//もし攻撃対象をBlackBoardに設定していなかったら設定する
	if (BB_Enemy && AIC_Enemy) {
		UObject* target = BB_Enemy->GetValueAsObject(AIC_Enemy->TargetName);
		if (target == nullptr && DamageCauser != nullptr) {
			BB_Enemy->SetValueAsObject(AIC_Enemy->TargetName, DamageCauser);

			//攻撃対象の方へ向く
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation());
			SetActorRotation(LookAtRotation);
		}
	}

	//死亡処理
	if (!Health->GetIsAlive()) {
		//AIControllerを外して動きを止める
		GetController()->Destroy();

		//付属のActor達をDestroyする
		if (HPWidget)HPWidget->DestroyComponent();
		if (SoundRing)SoundRing->DestroyMySelf();

		//Enemy本体はまだdestroyしないが当たり判定をなくす
		UCapsuleComponent* MyCapsule = GetCapsuleComponent();
		MyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && Die_Montage)
		{
			// モンタージュを再生
			AnimInstance->Montage_Play(Die_Montage);

			// ブレンドアウト開始時のイベントをバインド
			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindUObject(this, &AACharacterBase::OnMontageBlendingOut);

			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, Die_Montage);
		}
	}
}

void AACharacterBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->bPauseAnims = false;
	GetMesh()->SetVisibility(false);
	if (Die_Particle) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Die_Particle,
			GetMesh(),
			NAME_None,
			FVector(0.f),
			FRotator(0.f),
			EAttachLocation::SnapToTarget,
			true
		);

		//ここでつぎのステージへのドアを開けるためのデリゲートを発火
		OnEnemyDead.Broadcast();

		//ちょっとめんどいけど、Dieパーティクルが終わったらEnemyをDestroyする
		if (NiagaraComp) {
			NiagaraComp->OnSystemFinished.AddDynamic(
				this,
				&AACharacterBase::OnDieParticleFinished
			);
		}

		//リワードのクリスタルを放出する
		for (int i = 0; i < RewardNum; i++) {
			ARewardCrystal* reward = GetWorld()->SpawnActor<ARewardCrystal>(
				RewardItem, GetActorLocation(), GetActorRotation());

			reward->StartSpreadOut();
		}
	}
}

void AACharacterBase::OnDieParticleFinished(UNiagaraComponent* PSystem) {
	Destroy();
}

// Called to bind functionality to input
void AACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AACharacterBase::GetEnemyForwardVector() const {
	return RootComponent->GetForwardVector();
}

bool AACharacterBase::DamageToEnemy(int32 DamageValue, AActor* DamageCauser, bool IsComboHit) {
	AMainCharacter* player = Cast<AMainCharacter>(DamageCauser);
	if (player) {
		UGameplayStatics::ApplyDamage(
			this,                 // ダメージを受けるアクター（この敵自身）
			(float)DamageValue,   // ダメージ量
			player->GetController(), // ダメージを与えた人のコントローラー
			DamageCauser,         // ダメージを引き起こしたアクター
			UDamageType::StaticClass() // ダメージタイプ（基本はこれでOK）
		);

		return true;
	}

	return false;
}

bool AACharacterBase::CanDamage() {
	return true;
}

void AACharacterBase::ApplyKnockBack(FVector SourceLocation, float Stlength) {
	FVector KBVector = GetActorLocation() - SourceLocation;
	KBVector.Z = 0;		//水平方向だけのベクトルにする
	KBVector.Normalize();

	//Enemyにノックバックのための力を加える
	LaunchCharacter(KBVector * Stlength, true, true);

}

void AACharacterBase::StartAttackAnim() {
	if (!AttackMontage) {
		return;
	}
	float duration = RequestStateMontage(EEnemyState::Attacking, AttackMontage);
}

bool AACharacterBase::RequestStateMontage(EEnemyState NewState, UAnimMontage* MontageToPlay) {
	// 現在の状態より優先度が低い場合は拒否
	if (NewState < CurrentState)
	{
		return false;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MontageToPlay)
	{
		// アニメーション再生
		AnimInstance->Montage_Play(MontageToPlay);
		CurrentState = NewState;

		StopAIBehavior();

		// 終了時の処理をバインド
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AACharacterBase::OnMontageFinished);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);


		return true;
	}
	return false;
}

void AACharacterBase::OnMontageFinished(UAnimMontage* Montage, bool bInterrupted) {
	if (!bInterrupted)
	{
		CurrentState = EEnemyState::None;
		ResumeAIBehavior();
	}
}

void AACharacterBase::StopAIBehavior() {
	if (AIC_Enemy)
	{
		// 【物理的な停止】現在実行中の移動パスをキャンセルし、足を止める
		AIC_Enemy->StopMovement();

		// 【論理的な停止】Behavior Tree の実行を一時停止する
		if (UBrainComponent* BrainComp = AIC_Enemy->GetBrainComponent())
		{
			// "StateChange" という理由でロジックを一時停止（Pause）
			BrainComp->PauseLogic(TEXT("StateChange"));
		}
	}
}

void AACharacterBase::ResumeAIBehavior() {
	if (AIC_Enemy)
	{
		if (UBrainComponent* BrainComp = AIC_Enemy->GetBrainComponent())
		{
			// 一時停止していたロジックを再開
			BrainComp->ResumeLogic(TEXT("StateChange"));
		}
	}
}

void AACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Log, TEXT("Destroy Enemy"));
}

void AACharacterBase::UnBindAITarget() {
	if (BB_Enemy && AIC_Enemy) {
		BB_Enemy->SetValueAsObject(AIC_Enemy->TargetName, nullptr);

		UE_LOG(LogTemp, Log, TEXT("UnBind Target!"));
	}
}

AActor* AACharacterBase::CheckSensor() {
	bool isHit = Sensor->CheckSensor(CheckHitResults);

	if (isHit) {
		for (int i = 0; i < CheckHitResults.Num(); i++) {
			AMainCharacter* player = Cast<AMainCharacter>(CheckHitResults[i]);
			if (player) {
				return player;
			}
		}
	}

	return nullptr;
}
