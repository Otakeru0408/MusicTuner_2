// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_DamageText.h"

void UWidget_DamageText::PlayAnim(int DamageNum, int ComboIndex) {
	if (damageText) {
		damageText->SetText(FText::AsNumber(DamageNum));
	}
	UWidgetAnimation* nowAnim = nullptr;

	switch (ComboIndex)
	{
	case 0:
		nowAnim = Anim1;
		break;
	case 1:
		nowAnim = Anim2;
		break;
	case 2:
		nowAnim = Anim3;
		break;
	default:
		nowAnim = nullptr;
		break;
	}

	// Widget自身の終了イベントに自分の関数を紐付ける
	FWidgetAnimationDynamicEvent EndEvent;
	EndEvent.BindDynamic(this, &UWidget_DamageText::Internal_OnFinished);
	BindToAnimationFinished(nowAnim, EndEvent);

	PlayAnimation(nowAnim);
}

void UWidget_DamageText::Internal_OnFinished()
{
	//アニメーションが終わったら、DamagePopupに向けて通知を発火
	OnMyAnimationFinished.Broadcast();
}