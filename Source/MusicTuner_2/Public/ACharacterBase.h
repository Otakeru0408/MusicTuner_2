// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h" //追加
#include "Kismet/KismetStringLibrary.h" //追加
#include "DamageTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h" // Behavior Tree の制御に必要
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "ACharacterBase.generated.h"

class ASoundRingNext;
class UHealthComponent;
class AEnemyAIController;
class UPlayerHP;
class ARewardCrystal;
class USensor;
class ADamagePopup;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None,
	Idle,
	Move,
	Attacking,
	Defending, // ひるみ（攻撃より優先）
};

UCLASS()
class MUSICTUNER_2_API AACharacterBase : public ACharacter, public IDamageTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACharacterBase();

	//IDamageTargetの実装
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual bool DamageToEnemy(int32 DamageValue, AActor* DamageCauser, bool IsComboHit)override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual bool CanDamage()override;

	UFUNCTION()
	float GetDamageNum() { return DamageNum; }

	// 戻り値で再生されたかどうかを返す
	UFUNCTION(BlueprintCallable)
	bool RequestStateMontage(EEnemyState NewState, UAnimMontage* MontageToPlay);

	UFUNCTION()
	void ApplyKnockBack(FVector SourceLocation, float Stlength);


	UFUNCTION()
	AActor* CheckSensor();


	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<USensor> Sensor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnDieParticleFinished(UNiagaraComponent* PSystem);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void StopAIBehavior();

	UFUNCTION()
	void ResumeAIBehavior();

	UFUNCTION()
	void UnBindAITarget();



	UPROPERTY(VisibleAnywhere, Category = "Health")
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HPWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHP> Widget_HP_Class;

	UPROPERTY(EditAnywhere, Category = "UI")
	float DamageTextOffset_Z = 50.0f;

	UPROPERTY()
	TObjectPtr<UPlayerHP> WidgetData;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr < AEnemyAIController> AIC_Enemy;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr < UBlackboardComponent> BB_Enemy;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<TObjectPtr<AActor>> PatrolLocations;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TArray<TObjectPtr<AActor>> CheckHitResults;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float DamageNum = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	EEnemyState CurrentState = EEnemyState::Idle;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf <ADamagePopup> DamagePopupUI;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TObjectPtr<UAnimMontage> Die_Montage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TObjectPtr<UAnimMontage> Defend_Montage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TObjectPtr<UNiagaraSystem> Die_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TSubclassOf<ASoundRingNext> SoundRingClass;

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	TObjectPtr<ASoundRingNext> SoundRing;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FVector SoundRingOffset;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	int RewardNum = 5;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	TSubclassOf <ARewardCrystal> RewardItem;

	UPROPERTY(EditAnywhere, Category = "SoundRing")
	int BPM;

	UPROPERTY(EditAnywhere, Category = "SoundRing")
	int RingDividedNum;

	UPROPERTY(EditAnywhere, Category = "SoundRing")
	int RingNum;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	FVector GetEnemyForwardVector()const;

	UFUNCTION(BlueprintCallable)
	void StartAttackAnim();

};
