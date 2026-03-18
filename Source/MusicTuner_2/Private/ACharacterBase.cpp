// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacterBase.h"
#include "SoundRingNext.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHP.h"

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

	// デフォルトの設定をコードで入れておくとBPで楽になります
	HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidget->SetDrawAtDesiredSize(true);
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
		}
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
}

// Called every frame
void AACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!BB_Enemy)BB_Enemy = AIC_Enemy->GetBlackboardComponent();
}

//ダメージを受けた時の関数
void AACharacterBase::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	UE_LOG(LogTemp, Log, TEXT("Damaged!"));

	//HPを更新する
	Health->UpdateHP(Damage);

	//HPのUIを更新する
	if (WidgetData)WidgetData->UpdateHP(Health->GetCurrentPercent());

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

		// ログ出力（デバッグ用）
		//UE_LOG(LogTemp, Log, TEXT("%s took damage from CharacterBase!"), *GetName());

		return true;
	}

	return false;
}

bool AACharacterBase::CanDamage() {
	return true;
}