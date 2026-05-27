// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"     // 追加する
#include "GameFramework/CharacterMovementComponent.h" // 必須インクルード

#include "MainCharacter.generated.h"


class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class UHealthComponent;
class UAudioComponent;
class UPlayerHP;
class UWidget_PlayerDeath;

//プレイヤー死亡時に発動するデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);

UCLASS()
class MUSICTUNER_2_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDied OnPlayerDied;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int32 DamageCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int32 ComboCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	int32 MaxComboCount = 2;

	UPROPERTY(VisibleDefaultsOnly, Category = "Attack")
	bool isAttacking = false;

	UFUNCTION()
	bool CheckHitCount();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartCameraShake();


	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayHitSound();

	UFUNCTION()
	float GetDamageNum() { return DefaultDamageNum; }

	UFUNCTION()
	float GetNowDamageNum(int enemyPitch);

	UFUNCTION()
	void OnCrystalGained();

	UFUNCTION()
	float GetKnockBackPower() { return KnockBackPower; }

	UFUNCTION()
	bool GetIsAlive();

	UFUNCTION(BlueprintPure)
	UPlayerHP* GetPlayerUI() { return Widget_HP; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void QuitGame(const FInputActionValue& Value);
	void ControlMovement(const FInputActionValue& Value);
	void DodgeRollMovement(const FInputActionValue& Value);
	void ControlCamera(const FInputActionValue& Value);
	void SetMoveToDash(const FInputActionValue& Value) {
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
	void SetMoveToWalk(const FInputActionValue& Value) {
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	void EventOnAttack(const FInputActionValue& value);
	void Tuning(const FInputActionValue& Value);
	void FinishTuning(const FInputActionValue& Value);

	bool GetIsAudioPlaying();

	//いったん仮に死亡通知を受け取る関数
	UFUNCTION()
	void PlayerDeathLeastener();

	//実際に死亡処理を行う関数
	UFUNCTION()
	void OnPlayerDeath();

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float HarmonyTable[12] =
	{
		2.0f, // 0  同音（ユニゾン）
		0.5f, // 1  短2度（強い不協和）
		0.7f, // 2  長2度
		1.6f, // 3  短3度（協和）
		1.8f, // 4  長3度（強い協和）
		1.5f, // 5  完全4度
		0.5f, // 6  トライトーン（最も不協和）
		2.0f, // 7  完全5度（最強協和）
		1.6f, // 8  短6度
		1.8f, // 9  長6度
		0.7f, // 10 短7度
		0.6f  // 11 長7度（不安定）
	};

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UPlayerHP> Widget_HP_Class;

	UPROPERTY()
	TObjectPtr<UPlayerHP> Widget_HP;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UWidget_PlayerDeath> Widget_Death_Class;

	UPROPERTY()
	TObjectPtr<UWidget_PlayerDeath> Widget_Death;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	TObjectPtr<UHealthComponent> Health;

	/** Cameraを配置するためのSpringArm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm;

	/** SpringArmの先端に配置するカメラ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TArray<TObjectPtr<UAudioComponent>> AudioArray;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<UAudioComponent> Audio;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<UAudioComponent> Audio2;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<UAudioComponent> Audio3;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Control Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ControlAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CameraRotateAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DashFlag;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> QuitAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> TuningAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAnimMontage> DodgeRollMontage;

	UPROPERTY(EditAnywhere, Category = "Input")
	float Speed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float rotSpeedX = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float rotSpeedY = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float HP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<int32> SoundIndexArray = { 0,3,8 };

	UPROPERTY(EditAnywhere, Category = "Sound")
	int TuneDiff = 41;		//C=48だが、最初にチューニングさせたいから41(G)にしている

	UPROPERTY(EditAnywhere, Category = "Sound")
	float SoundContinuousTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<float> DamageNumByCombo;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float DefaultDamageNum = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float KnockBackPower = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Reward")
	int RewardCrystalNum = 0;

	UPROPERTY()
	TArray<float> DoubleClickTimeWASD = { 0.f,0.f,0.f,0.f };

	UPROPERTY(EditAnywhere, Category = "Input")
	float DoubleClickLimitTime = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float CrystalHealVal = 1.0f;

private:
	TArray<FTimerHandle> SoundStopTimerHandles;
};
