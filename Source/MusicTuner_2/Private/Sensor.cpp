// Fill out your copyright notice in the Description page of Project Settings.


#include "Sensor.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USensor::USensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USensor::BeginPlay()
{
	Super::BeginPlay();

	//トレースの長さと角度から縦(横)の長さを計算する
	BoxWidth = TraceLength * FMath::Tan(FMath::DegreesToRadians(TraceAngle * 0.5f));
	Owner = GetOwner();
}


bool USensor::CheckSensor(TArray<TObjectPtr<AActor>>& OutResults) {
	OutResults.Empty();		//いったん初期化。ここにhitしたアクターを詰めていく

	FVector StartLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * TraceLength * 0.5f;
	//FVector EndLocation = StartLocation + Owner->GetActorForwardVector() * TraceLength;
	FQuat Rot = Owner->GetActorQuat();
	FCollisionShape Box = FCollisionShape::MakeBox(FVector(TraceLength / 2.0f, BoxWidth, BoxWidth));

	//BoxTraceには2つの考え方があるらしい
	//パターンA:TraceLengthの長さのBoxを用意して、StartとEndをおなじ位置にする
	//パターンB:Boxの厚みを1にして、そのBoxをStartからEndまで移動させる
	//今回はパターンAにします。

	TArray<FHitResult> OutHits;


	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		StartLocation,
		StartLocation,
		Rot,
		ECC_Camera,
		Box,
		FCollisionQueryParams::DefaultQueryParam
	);

	//検出範囲の視覚化
	DrawDebugBox(
		Owner->GetWorld(),
		StartLocation,
		FVector(TraceLength / 2.0f, BoxWidth, BoxWidth),
		Rot,
		FColor::Yellow
	);

	/*if (!bHit) { UE_LOG(LogTemp, Log, TEXT("dont hit Sensor")); }
	else { UE_LOG(LogTemp, Log, TEXT("Hit Sensor")); }*/


	if (bHit) {
		for (const FHitResult& Hit : OutHits)
		{
			if (AActor* Actor = Hit.GetActor())
			{
				// 重複を避けたい場合は AddUnique、単純追加なら Add
				OutResults.AddUnique(Actor);
			}
		}
	}

	return bHit;
}