// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"  // 追加する
#include "EnhancedInputSubsystems.h" // 追加する
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h" // 必須インクルード

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
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ControlAction, ETriggerEvent::Triggered, this, &AMainCharacter::ControlMovement);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AMainCharacter::ControlCamera);
		EnhancedInputComponent->BindAction(DashFlag, ETriggerEvent::Started, this, &AMainCharacter::SetMoveToDash);
		EnhancedInputComponent->BindAction(DashFlag, ETriggerEvent::Completed, this, &AMainCharacter::SetMoveToWalk);
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