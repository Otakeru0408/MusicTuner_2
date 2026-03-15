// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h" //’Ç‰Á
#include "Kismet/KismetStringLibrary.h" //’Ç‰Á
#include "DamageTarget.h"

#include "ACharacterBase.generated.h"

class ASoundRingNext;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TSubclassOf<ASoundRingNext> SoundRingClass;

	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	TObjectPtr<ASoundRingNext> SoundRing;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FVector SoundRingOffset;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	FVector GetEnemyForwardVector()const;

};
