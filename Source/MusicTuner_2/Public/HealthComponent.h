// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MUSICTUNER_2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION()
	void UpdateHP(float Damage);

	UFUNCTION()
	float GetCurrectHP() { return CurrentHealth; }

	UFUNCTION()
	float GetMaxHP() { return MaxHealth; }

	UFUNCTION()
	float GetCurrentPercent() { return CurrentHealth / MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// ダメージを受け取った時のハンドラ（UFUNCTIONが必須）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	// 現在のHP割合を返す（UI制作などで便利）
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }
};
