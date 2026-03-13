// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTarget.h"

// Sets default values
AEnemyTarget::AEnemyTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	if (HitUnableMat) {
		Sphere->SetMaterial(0, HitUnableMat);
	}
}

// Called when the game starts or when spawned
void AEnemyTarget::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forward = Sphere->GetForwardVector();
	FVector NextPos = forward * Speed * DeltaTime + GetActorLocation();
	SetActorLocation(NextPos);

	//プレイヤーに一定期間でダメージを与え続けるための変数
	if (DamageTime > 0) {
		DamageTime -= DeltaTime;
	}

	//UE_LOG(LogTemp, Display, TEXT("Distance : %f"), CulcDistanceToParent());

	//Parentから一定範囲内はダメージを与えられないようにするが、そこを超えたらダメージできるようにする
	if (CulcDistanceToParent() >= NoDamageRange && HitEnableMat) {
		Sphere->SetMaterial(0, HitEnableMat);
	}
	else {
		Sphere->SetMaterial(0, HitUnableMat);
	}


}

float AEnemyTarget::CulcDistanceToParent() {
	if (!GetOwner())return 0.0f;

	FVector VectorToParent = GetOwner()->GetActorLocation() - GetActorLocation();
	return VectorToParent.Length();
}
