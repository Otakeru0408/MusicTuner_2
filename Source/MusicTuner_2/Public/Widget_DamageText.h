// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "Widget_DamageText.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationFinishedSignature);

UCLASS()
class MUSICTUNER_2_API UWidget_DamageText : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayAnim(int DamageNum, int ComboIndex);

	// 2. äOïîÇ©ÇÁìoò^Ç≈Ç´ÇÈÇÊÇ§Ç…Ç∑ÇÈïœêî
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAnimationFinishedSignature OnMyAnimationFinished;

protected:
	UFUNCTION()
	void Internal_OnFinished();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* damageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim1;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim2;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim3;
};
