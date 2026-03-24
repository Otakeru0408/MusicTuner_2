// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "JumpPad.generated.h"

UCLASS()
class MUSICTUNER_2_API AJumpPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJumpPad();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 1. コンポーネント群
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> OverlapVolume;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USplineComponent> SplinePath;

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	TObjectPtr<UAnimMontage> JumpMontage;

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float JumpTime = 1.0f;

	// 2. タイムライン関連
	FTimeline JumpTimeline;

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	class UCurveFloat* JumpCurve; // エディタで作成したカーブをセット

	// 3. 実行中に保持するプレイヤー参照
	class ACharacter* TargetCharacter;

	// タイムラインのアップデートで呼ばれる関数 (UFUNCTION必須)
	UFUNCTION()
	void HandleJumpProgress(float Value);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnJumpFinished();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
