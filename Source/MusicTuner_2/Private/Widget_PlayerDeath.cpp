// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PlayerDeath.h"
#include "Kismet/GameplayStatics.h"

bool UWidget_PlayerDeath::Initialize() {
	if (!Super::Initialize())
	{
		return false;
	}

	// ボタンが正しくバインドされているか確認してバインド
	if (BT_Restart)
	{
		BT_Restart->OnClicked.AddDynamic(this, &UWidget_PlayerDeath::OnRestartButtonClicked);
	}

	if (BT_Title) {
		BT_Title->OnClicked.AddDynamic(this, &UWidget_PlayerDeath::OnTitleButtonClicked);
	}

	return true;
}

void UWidget_PlayerDeath::OnRestartButtonClicked() {
	FString levelname = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*levelname));
}

void UWidget_PlayerDeath::OnTitleButtonClicked() {
	UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
}