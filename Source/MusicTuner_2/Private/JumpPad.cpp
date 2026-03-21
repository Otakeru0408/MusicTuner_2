// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJumpPad::AJumpPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	OverlapVolume->SetupAttachment(RootComponent);

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnOverlapBegin);

	// タイムラインの設定
	if (JumpCurve)
	{
		FOnTimelineFloat ProgressUpdate;
		ProgressUpdate.BindUFunction(this, FName("HandleJumpProgress"));
		JumpTimeline.AddInterpFloat(JumpCurve, ProgressUpdate);

		// 終了時
		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("OnJumpFinished"));
		JumpTimeline.SetTimelineFinishedFunc(FinishedEvent);
	}
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	JumpTimeline.TickTimeline(DeltaTime);
}

void AJumpPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap to Box"));
	TargetCharacter = Cast<ACharacter>(OtherActor);
	if (TargetCharacter && !JumpTimeline.IsPlaying())
	{
		if (JumpMontage) {
			TargetCharacter->PlayAnimMontage(JumpMontage);
		}
		// ジャンプ開始！(移動モードを一時的に変更して物理干渉を防ぐ)
		TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		JumpTimeline.PlayFromStart();
	}
}

void AJumpPad::HandleJumpProgress(float Value)
{
	if (TargetCharacter && SplinePath)
	{
		// スプラインの全長に対して、カーブの値(0~1)を掛けて現在の位置を取得
		float Distance = SplinePath->GetSplineLength() * Value;
		FVector NewLocation = SplinePath->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		// プレイヤーの位置を更新
		TargetCharacter->SetActorLocation(NewLocation);

		// オプション：進行方向にプレイヤーの向きを合わせる
		/*FRotator NewRotation = SplinePath->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		TargetCharacter->SetActorRotation(NewRotation);*/
	}
}

void AJumpPad::OnJumpFinished()
{
	if (TargetCharacter && TargetCharacter->GetCharacterMovement())
	{
		if (JumpMontage) {
			TargetCharacter->StopAnimMontage(JumpMontage);
		}
		// 移動モードを「落下中」に戻すことで、重力が計算されるようになる
		TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);

		// 参照をクリア（次のジャンプに備える）
		TargetCharacter = nullptr;
	}
}
