// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyTarget.generated.h"

UCLASS()
class MUSICTUNER_2_API AEnemyTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyTarget();
	void SetSpeed(float sp) {
		Speed = sp;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//SoundRingÇ∆ÇÃãóó£ÇåvéZÇ∑ÇÈä÷êî
	UFUNCTION(BlueprintPure)
	float CulcDistanceToParent();

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parameter")
	TObjectPtr<AActor> ParentActor;*/

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Sphere;

	UPROPERTY(VisibleAnywhere, Category = "Parameter")
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float DamageTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float DamageMaxTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float NoDamageRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UMaterialInterface> HitUnableMat;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UMaterialInterface> HitEnableMat;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
