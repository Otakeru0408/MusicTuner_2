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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Sphere;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float Speed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
