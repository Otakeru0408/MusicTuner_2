// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"

#include "DamagePopup.generated.h"

class UWidget_DamageText;

UCLASS()
class MUSICTUNER_2_API ADamagePopup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamagePopup();

	UPROPERTY(VisibleAnywhere, Category = "UI")
	int32 DamageNum = 0;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	int32 ComboIndex = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAnimFinished();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> DamageWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWidget_DamageText> Widget_Damage_Class;

	UPROPERTY()
	TObjectPtr<UWidget_DamageText> WidgetData;

};
