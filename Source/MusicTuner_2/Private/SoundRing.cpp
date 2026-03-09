// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundRing.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ASoundRing::ASoundRing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	RootComponent = Disc;
	Disc->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void ASoundRing::BeginPlay()
{
	Super::BeginPlay();

	//RingNum→RingCountの変換
	//実際には3という数字で2本になり5で3本になるので、実際に欲しい本数(RingNum)から提出する数値(RingCount)に変換する
	RingCount = RingNum * 2 - 1;

	//現在のDiscのScale,RingCountから2つの波のあいだの距離を計算する
	//50はDiscのScale=1の時の半径のサイズ。DiscのScaleはどれも一緒なのでXを使用
	SoundRingRadius_Single = (50.0 / RingCount) * Disc->GetRelativeScale3D().X;
	SoundRingRadius_All = 50 * Disc->GetRelativeScale3D().X;

	//Ringの広がるスピードを計算する
	//RingNumがなんぼでもBPM=60なら1秒に一回外側にRingが来る
	RingSpeed = (BPM / 60.0) * SoundRingRadius_Single * 2;

	if (M_SoundRing) {
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(M_SoundRing.Get(), this);
		DynamicMaterialInst->SetScalarParameterValue("TimeValue", 0);
		DynamicMaterialInst->SetScalarParameterValue("BPM", BPM);
		DynamicMaterialInst->SetScalarParameterValue("Size", RingCount);

		Disc->SetMaterial(0, DynamicMaterialInst);
	}
}

// Called every frame
void ASoundRing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PassTime += DeltaTime;
	DynamicMaterialInst->SetScalarParameterValue("TimeValue", PassTime);
}

