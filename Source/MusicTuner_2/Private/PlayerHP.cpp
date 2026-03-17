// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHP.h"


void UPlayerHP::InitData(float maxHP) {
	//まずは最大のWとHを取得する
	if (HP_Bar)
	{
		// SlotをCanvasPanelSlotとして取得
		HP_Bar_Slot = Cast<UCanvasPanelSlot>(HP_Bar->Slot);
		if (HP_Bar_Slot)
		{
			MaxBarSize = HP_Bar_Slot->GetSize();
		}
	}

	if (HP_LateBar)
	{
		// SlotをCanvasPanelSlotとして取得
		HP_LateBar_Slot = Cast<UCanvasPanelSlot>(HP_LateBar->Slot);
	}

	if (HP_Text) {
		// 1. float を文字列（FText）に変換
		// FText::Format を使うと「100 / 100」のような整形が楽です
		FText HPFormattedText = FText::Format(
			FText::FromString("HP : {0}"),
			FText::AsNumber(FMath::CeilToInt(maxHP))
		);

		// 2. テキストをセット
		HP_Text->SetText(HPFormattedText);
	}
}

void UPlayerHP::UpdateHP(float HealthPercent) {
	if (HP_Bar_Slot) {
		HP_Bar_Slot->SetSize(FVector2D(MaxBarSize.X * HealthPercent, MaxBarSize.Y));
	}
	GetWorld()->GetTimerManager().SetTimer(
		SoundStopTimerHandle, [this, HealthPercent]()
		{
			if (HP_LateBar_Slot) {
				HP_LateBar_Slot->SetSize(FVector2D(MaxBarSize.X * HealthPercent, MaxBarSize.Y));
			}
		},
		DamageDelayTime, false
	);
}