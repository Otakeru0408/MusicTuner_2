// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"

#include "PlayerHP.generated.h"

UCLASS()
class MUSICTUNER_2_API UPlayerHP : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* HP_Bar;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HP_Text;

	UPROPERTY()
	FVector2D MaxBarSize;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> HP_Bar_Slot;

	UFUNCTION(BlueprintCallable)
	void UpdateHP(float HealthPercent);

	UFUNCTION()
	void InitData(float maxHP);
};
