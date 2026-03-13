// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"  // 追加する
#include "EnhancedInputSubsystems.h" // 追加する
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h" // 必須インクルード

// Sets default values
ABallPlayer::ABallPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//カプセルコンポーネント(RootComponent)
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Capsule;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetupAttachment(Capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(Capsule);

	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.f, 0.f));

	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	Camera->PostProcessSettings.MotionBlurAmount = 0.0f;

	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Player"));
	ControlAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/InputActions/IA_PlayerMove"));


}

// Called when the game starts or when spawned
void ABallPlayer::BeginPlay()
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


// Called to bind functionality to input
void ABallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ControlAction, ETriggerEvent::Triggered, this, &ABallPlayer::ControlBall);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &ABallPlayer::ControlCamera);
		UE_LOG(LogTemp, Display, TEXT("Change Rotation"));
	}
}

void ABallPlayer::ControlBall(const FInputActionValue& Value) {
	const FVector2D V = Value.Get<FVector2D>();

	FVector ForceVector = Camera->GetForwardVector() * V.X * Speed + Camera->GetRightVector() * V.Y * Speed;

	//Sphere->AddForce(ForceVector, NAME_None, true);
	FVector nowPos = GetActorLocation();
	SetActorLocation(FVector(nowPos.X + ForceVector.X, nowPos.Y + ForceVector.Y, nowPos.Z));
}

void ABallPlayer::ControlCamera(const FInputActionValue& Value) {
	// マウスの移動量（Vector2D）を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 現在の回転を取得して、マウスの移動量を加算する
	FRotator NewRotation = SpringArm->GetRelativeRotation();
	NewRotation.Pitch += LookAxisVector.Y * rotSpeed;
	NewRotation.Yaw += LookAxisVector.X * rotSpeed;

	// Pitchの範囲を制限（ひっくり返らないように）
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.f, 20.f);

	SpringArm->SetRelativeRotation(NewRotation);

}