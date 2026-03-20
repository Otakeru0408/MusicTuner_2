// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePopup.h"
#include "Widget_DamageText.h"

// Sets default values
ADamagePopup::ADamagePopup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	RootComponent = DamageWidget;

	DamageWidget->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidget->SetDrawAtDesiredSize(true);
}

// Called when the game starts or when spawned
void ADamagePopup::BeginPlay()
{
	Super::BeginPlay();
	if (Widget_Damage_Class) {
		DamageWidget->SetWidgetClass(Widget_Damage_Class);
		WidgetData = Cast<UWidget_DamageText>(DamageWidget->GetUserWidgetObject());
		WidgetData->OnMyAnimationFinished.AddDynamic(this, &ADamagePopup::OnAnimFinished);

		WidgetData->PlayAnim(DamageNum, ComboIndex);
	}
}

void ADamagePopup::OnAnimFinished() {
	Destroy();
}

