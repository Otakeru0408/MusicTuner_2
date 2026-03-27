// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::UpdateHP(float Damage)
{
	if (Damage <= 0.0f || CurrentHealth <= 0.0f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		// ここで死亡通知を送る（デリゲートを飛ばすとさらに良い）
		UE_LOG(LogTemp, Warning, TEXT("%s Died!"), *GetOwner()->GetName());
		isAlive = false;
	}
}

void UHealthComponent::HealHP(float heal) {
	CurrentHealth += heal;
	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
}