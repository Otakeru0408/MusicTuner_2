// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "Widget_PlayerDeath.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API UWidget_PlayerDeath : public UUserWidget
{
	GENERATED_BODY()

public:
	// 初期化処理（ここでイベントをバインドする）
	virtual bool Initialize() override;

	UFUNCTION()
	void OnTitleButtonClicked();

	UFUNCTION()
	void OnRestartButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* BT_Title;

	UPROPERTY(meta = (BindWidget))
	class UButton* BT_Restart;
};
