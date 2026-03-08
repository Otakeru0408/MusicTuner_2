// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h" // ’Ç‰Á
#include "Components/PointLightComponent.h" // ’Ç‰Á

#include "AComponentStudy.generated.h"

UCLASS()
class MUSICTUNER_2_API AAComponentStudy : public AActor
{
	GENERATED_BODY()

public:
	AAComponentStudy();

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPointLightComponent> PointLight;

protected:
	virtual void BeginPlay() override;

private:
	const float Duration = 10.0f;

	const FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0);

};
