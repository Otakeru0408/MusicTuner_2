// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHP.h"


void UPlayerHP::InitData(float maxHP, int tuneDiff) {
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

		TuneDiff_Text->SetText(FText::Format(FText::FromString("TuneDiff : {0}"), tuneDiff));
	}

	if (CrystalText) {
		CrystalText->SetText(FText::FromString("Crystal : 0"));
	}
}

void UPlayerHP::UpdateHP(float HealthPercent, float HP) {
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

	if (HP_Text) {
		FText HPFormattedText = FText::Format(
			FText::FromString("HP : {0}"),
			FText::AsNumber(FMath::CeilToInt(HP))
		);

		// 2. テキストをセット
		HP_Text->SetText(HPFormattedText);
	}
}

void UPlayerHP::UpdateCrystalNum(int num) {
	CrystalText->SetText(FText::Format(FText::FromString("Crystal : {0}"), num));
}

void UPlayerHP::UpdateTuneDiffText(int num) {
	TuneDiff_Text->SetText(FText::Format(FText::FromString("TuneDiff : {0}"), num));
}