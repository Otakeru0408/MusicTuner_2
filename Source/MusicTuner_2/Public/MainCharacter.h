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
class UInputMappingContext;       // 追加する
class UInputAction;               // 追加する
class UCapsuleComponent;
class UHealthComponent;

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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ControlMovement(const FInputActionValue& Value);
	void ControlCamera(const FInputActionValue& Value);
	void SetMoveToDash(const FInputActionValue& Value) {
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
	void SetMoveToWalk(const FInputActionValue& Value) {
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	void EventOnAttack(const FInputActionValue& value);

	UPROPERTY(VisibleAnywhere, Category = "Health")
	TObjectPtr<UHealthComponent> Health;

	/** Cameraを配置するためのSpringArm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm;

	/** SpringArmの先端に配置するカメラ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

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

	UPROPERTY(EditAnywhere, Category = "Input")
	float Speed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float rotSpeedX = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float rotSpeedY = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float HP = 100.0f;
};
