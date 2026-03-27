// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	float GetNowDamageNum();

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

	//いったん仮に死亡通知を受け取る関数
	UFUNCTION()
	void PlayerDeathLeastener();

	//実際に死亡処理を行う関数
	UFUNCTION()
	void OnPlayerDeath();

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
	TObjectPtr<UAudioComponent> Audio;

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
	TArray<int32> SoundIndexArray = { 52,55,60 };

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
	FTimerHandle SoundStopTimerHandle;
};
