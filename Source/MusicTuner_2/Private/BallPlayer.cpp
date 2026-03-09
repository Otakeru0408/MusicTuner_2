// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"  // ’Ç‰Á‚·‚é
#include "EnhancedInputSubsystems.h" // ’Ç‰Á‚·‚é
#include "InputMappingContext.h"
#include "InputActionValue.h"

// Sets default values
ABallPlayer::ABallPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = Sphere;

	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	Sphere->SetStaticMesh(mesh);

	UMaterial* material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	Sphere->SetMaterial(0, material);

	Sphere->SetSimulatePhysics(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(Sphere);

	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.f, 0.f));

	SpringArm->TargetArmLength = 450.0f;
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
	}
}

void ABallPlayer::ControlBall(const FInputActionValue& Value) {
	const FVector2D V = Value.Get<FVector2D>();

	FVector ForceVector = FVector(V.Y, V.X, 0.0f) * Speed;

	//Sphere->AddForce(ForceVector, NAME_None, true);
	FVector nowPos = GetActorLocation();
	SetActorLocation(FVector(nowPos.X + ForceVector.X, nowPos.Y + ForceVector.Y, nowPos.Z));
}
