// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h" //’Ç‰Á
#include "Kismet/KismetStringLibrary.h" //’Ç‰Á
#include "DamageTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"

#include "ACharacterBase.generated.h"

class ASoundRingNext;
class UHealthComponent;
class AEnemyAIController;
class UPlayerHP;

UCLASS()
class MUSICTUNER_2_API AACharacterBase : public ACharacter, public IDamageTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACharacterBase();

	//IDamageTarget‚ÌŽÀ‘•
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual bool DamageToEnemy(int32 DamageValue, AActor* DamageCauser, bool IsComboHit)override;
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual bool CanDamage()override;

	UFUNCTION()
	float GetDamageNum() { return DamageNum; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, Category = "Health")
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HPWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHP> Widget_HP_Class;

	UPROPERTY()
	TObjectPtr<UPlayerHP> WidgetData;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr < AEnemyAIController> AIC_Enemy;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr < UBlackboardComponent> BB_Enemy;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<TObjectPtr<AActor>> PatrolLocations;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float DamageNum = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TObjectPtr<UAnimMontage> Die_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TSubclassOf<ASoundRingNext> SoundRingClass;

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	TObjectPtr<ASoundRingNext> SoundRing;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FVector SoundRingOffset;

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
