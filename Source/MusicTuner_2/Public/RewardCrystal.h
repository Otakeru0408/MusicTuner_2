// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "RewardCrystal.generated.h"

UCLASS()
class MUSICTUNER_2_API ARewardCrystal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARewardCrystal();

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase>  GainedSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> CrystalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
