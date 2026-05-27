// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"  // 追加する
#include "EnhancedInputSubsystems.h" // 追加する
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h" // 必須インクルード
#include "Components/AudioComponent.h" // 必須インクルード
#include "HealthComponent.h"
#include "DamageCameraShake.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHP.h"
#include "Widget_PlayerDeath.h"
#include "SoundTuner_Instance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	//SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.f, 0.f));

	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	Camera->PostProcessSettings.MotionBlurAmount = 0.0f;

	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Player"));
	ControlAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/InputActions/IA_PlayerMove"));
	CameraRotateAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/InputActions/IA_CameraRotation"));

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	Audio->SetupAttachment(RootComponent);
	Audio->SetAutoActivate(false);
	AudioArray.Add(Audio);

	Audio2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent2"));
	Audio2->SetupAttachment(RootComponent);
	Audio2->SetAutoActivate(false);
	AudioArray.Add(Audio2);


	Audio3 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent3"));
	Audio3->SetupAttachment(RootComponent);
	Audio3->SetAutoActivate(false);
	AudioArray.Add(Audio3);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	SoundStopTimerHandles.SetNum(AudioArray.Num());
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (Widget_HP_Class) {
		Widget_HP = CreateWidget<UPlayerHP>(GetWorld(), Widget_HP_Class);
		if (Widget_HP) {
			Widget_HP->AddToViewport();

			Widget_HP->InitData(Health->GetMaxHP(), TuneDiff);
		}
	}



	//新しくレベルをロードしたらHPを引き継ぐ
	USoundTuner_Instance* GI = Cast<USoundTuner_Instance>(GetGameInstance());
	if (GI && GI->PlayerHP > 0) {
		Health->SetCurrectHP(GI->PlayerHP);
		Widget_HP->UpdateHP(Health->GetCurrentPercent(), Health->GetCurrectHP());
		RewardCrystalNum = GI->crystalNum;
		Widget_HP->UpdateCrystalNum(RewardCrystalNum);
	}

	OnPlayerDied.AddDynamic(this, &AMainCharacter::PlayerDeathLeastener);
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	USoundTuner_Instance* GI = Cast<USoundTuner_Instance>(GetGameInstance());
	if (GI) {
		GI->PlayerHP = Health->GetCurrectHP();
		GI->crystalNum = RewardCrystalNum;
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// 1. 親クラスの処理を呼び、実際に適用されるダメージ量を受け取る
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		// 2. HPを減らす
		//HealthCOmponentでなにかTakeDamageに登録している関数があるぞ？
		Health->UpdateHP(ActualDamage);

		Widget_HP->UpdateHP(Health->GetCurrentPercent(), Health->GetCurrectHP());

		//HPが0以下なら死亡通知を発火
		if (!Health->GetIsAlive())OnPlayerDied.Broadcast();
	}

	// 最終的なダメージ量を返すのが決まり
	return ActualDamage;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ControlAction, ETriggerEvent::Triggered, this, &AMainCharacter::ControlMovement);
		EnhancedInputComponent->BindAction(ControlAction, ETriggerEvent::Started, this, &AMainCharacter::DodgeRollMovement);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AMainCharacter::ControlCamera);
		EnhancedInputComponent->BindAction(DashFlag, ETriggerEvent::Started, this, &AMainCharacter::SetMoveToDash);
		EnhancedInputComponent->BindAction(DashFlag, ETriggerEvent::Completed, this, &AMainCharacter::SetMoveToWalk);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::EventOnAttack);
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &AMainCharacter::QuitGame);
		EnhancedInputComponent->BindAction(TuningAction, ETriggerEvent::Started, this, &AMainCharacter::Tuning);
		EnhancedInputComponent->BindAction(TuningAction, ETriggerEvent::Completed, this, &AMainCharacter::FinishTuning);

	}
}

void AMainCharacter::ControlMovement(const FInputActionValue& Value) {
	const FVector2D V = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// カメラの向きに基づいた前方方向を計算
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// SetActorLocation の代わりに AddMovementInput を使用
		AddMovementInput(ForwardDirection, V.X);
		AddMovementInput(RightDirection, V.Y);

	}
}

void AMainCharacter::DodgeRollMovement(const FInputActionValue& Value) {
	const FVector2D V = Value.Get<FVector2D>();


	if (Controller != nullptr)
	{
		// カメラの向きに基づいた前方方向を計算
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// SetActorLocation の代わりに AddMovementInput を使用
		AddMovementInput(ForwardDirection, V.X);
		AddMovementInput(RightDirection, V.Y);

		//DodgeRollの処理
		int inputVal = 0;	//WASDのいずれであるか
		if (V.Y > 0)inputVal = 0;		//W
		else if (V.Y < 0)inputVal = 2;	//S
		else if (V.X > 0)inputVal = 3;	//D
		else if (V.X < 0)inputVal = 1;	//A

		if (GetWorld()->GetTimeSeconds() - DoubleClickTimeWASD[inputVal] < DoubleClickLimitTime) {
			//DodgeRollする方向に体を向かせる
			FVector inputDirection = ForwardDirection * V.X + RightDirection * V.Y;
			inputDirection.Normalize();
			SetActorRotation(FRotationMatrix::MakeFromX(inputDirection).Rotator());

			//Montageをplayする
			//Montageが終わったらisAttackingを解除することで攻撃が可能になる
			//MontageのAnimNotifyでDodgeRoll中はCollisionを敵やBallとぶつからないように設定している
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && DodgeRollMontage) {
				isAttacking = true;
				float duration = PlayAnimMontage(DodgeRollMontage);
				if (duration > 0) {
					FOnMontageEnded EndDelegate;
					EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
						{
							isAttacking = false;
						});

					AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeRollMontage);
				}
			}

		}
		else {
			DoubleClickTimeWASD[inputVal] = GetWorld()->GetTimeSeconds();
		}

	}

}

void AMainCharacter::ControlCamera(const FInputActionValue& Value) {
	// マウスの移動量（Vector2D）を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("Value:%f"), LookAxisVector.X);
		// X軸の移動（左右）→ Yaw（方位角）の回転
		AddControllerYawInput(LookAxisVector.X * rotSpeedX);

		// Y軸の移動（上下）→ Pitch（仰俯角）の回転
		// ※通常、マウスの上移動はプラスなので、反転させたい場合はマイナスを掛ける
		AddControllerPitchInput(LookAxisVector.Y * -1 * rotSpeedY);
	}
}

void AMainCharacter::EventOnAttack(const FInputActionValue& value) {
	//攻撃アニメーションがアタッチされていないか、攻撃制限中なら無し
	if (!AttackMontage || isAttacking) {
		return;
	}

	float duration = PlayAnimMontage(AttackMontage);
	isAttacking = true;		//攻撃モーション終了時に解除。これによって連続攻撃をさせない
}

//左右キーによって基音をチューニングする
void AMainCharacter::Tuning(const FInputActionValue& Value) {
	float input = Value.Get<float>();
	TuneDiff += input > 0.0f ? 1 : -1;

	Widget_HP->UpdateTuneDiffText(TuneDiff);

	AudioArray[0]->SetIntParameter(FName("Sound_ID"), SoundIndexArray[0] + TuneDiff);
	AudioArray[0]->Play();
}

//左右キーを押してから話すまでずっとその音を鳴らす
void AMainCharacter::FinishTuning(const FInputActionValue& Value) {
	AudioArray[0]->SetTriggerParameter(FName("On Stop"));
}

//CheckKickHitから呼び出される、攻撃時何かに当たった時のコンボ加算関数
bool AMainCharacter::CheckHitCount() {
	//MaxCombo=3回なので、3回目のコンボでまだ音が鳴ってるなら音の更新はしない
	//3回目のコンボは音が鳴り終えるまで待ってほしい
	//if (Audio->IsPlaying())UE_LOG(LogTemp, Log, TEXT("IsPlaying : %d"), ComboCount);
	if (GetIsAudioPlaying() && ComboCount == 2) return false;

	//音が鳴っていないときに攻撃したら1回目のコンボ
	//音が鳴っているときに攻撃したら、コンボ加算
	if (!GetIsAudioPlaying()) {
		ComboCount = 0;
	}
	else {
		ComboCount = (ComboCount + 1) % SoundIndexArray.Num();
	}
	return true;
}

void AMainCharacter::PlayHitSound() {
	for (int i = 0; i <= ComboCount; i++) {
		AudioArray[i]->SetIntParameter(FName("Sound_ID"), SoundIndexArray[i] + TuneDiff);
		AudioArray[i]->Play();
		GetWorldTimerManager().SetTimer(SoundStopTimerHandles[i], [this, i]()
			{
				AudioArray[i]->SetTriggerParameter(FName("On Stop"));
			},
			SoundContinuousTime, false);
	}
}


void AMainCharacter::StartCameraShake() {
	// 1. PlayerControllerを取得
	APlayerController* PC = Cast<APlayerController>(GetController());

	// 2. PCとCameraManagerが存在するかチェック
	if (PC && PC->PlayerCameraManager)
	{
		// 3. カメラシェイクを実行
		// 第2引数の Scale (1.0f) を調整することで、揺れの強さを動的に変えられます
		PC->PlayerCameraManager->StartCameraShake(UDamageCameraShake::StaticClass(), 1.0f);
	}
}

float AMainCharacter::GetNowDamageNum(int enemyPitch) {
	//EnemyTarget攻撃時にコンボに応じてダメージ数を増やす
	int nowIndex = 0;
	if (ComboCount < DamageNumByCombo.Num())nowIndex = ComboCount;

	//自身のPitchと敵のPitchを比べ、ハモる場合はダメージの割合を増やす
	//相手と自分の音程差を計算し、12の余りとする。
	int Interval = abs(TuneDiff - enemyPitch) % 12;

	//その音程差がどれだけハモるか取得
	float Harmony = HarmonyTable[Interval];

	UE_LOG(LogTemp, Display, TEXT("MyPitch:%d,EnemyPitch:%d, Harmony:%.2f"), TuneDiff, enemyPitch, Harmony);

	return DamageNumByCombo[nowIndex] * Harmony;
}

void AMainCharacter::OnCrystalGained() {
	RewardCrystalNum++;
	Health->HealHP(CrystalHealVal);
	Widget_HP->UpdateHP(Health->GetCurrentPercent(), Health->GetCurrectHP());
	Widget_HP->UpdateCrystalNum(RewardCrystalNum);
}

void AMainCharacter::PlayerDeathLeastener() {
	GetWorldTimerManager().SetTimerForNextTick(this, &AMainCharacter::OnPlayerDeath);
}

void AMainCharacter::OnPlayerDeath() {
	//入力を受け付けなくする
	if (APlayerController* controller = Cast<APlayerController>(GetController())) {
		DisableInput(controller);
		controller->bShowMouseCursor = true;
	}

	if (Widget_HP) {
		Widget_HP->RemoveFromParent();
	}

	//Meshをラグドール化する
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	//死亡時のUIを表示する
	if (Widget_Death_Class) {
		Widget_Death = CreateWidget<UWidget_PlayerDeath>(GetWorld(), Widget_Death_Class);
		if (Widget_Death) {
			Widget_Death->AddToViewport();

			Widget_Death->Initialize();
		}
	}

}

bool AMainCharacter::GetIsAlive() {
	return Health->GetIsAlive();
}

void AMainCharacter::QuitGame(const FInputActionValue& Value) {
	UKismetSystemLibrary::QuitGame(GetWorld(), Cast<APlayerController>(GetController()), EQuitPreference::Quit, false);
}

bool AMainCharacter::GetIsAudioPlaying() {
	for (int i = 0; i < AudioArray.Num(); i++) {
		if (AudioArray[i]->IsPlaying()) {
			return true;
		}
	}
	return false;
}