// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTarget.h"

// Sets default values
AEnemyTarget::AEnemyTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
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
	FVector NextPos = forward * Speed + GetActorLocation();
	SetActorLocation(NextPos);
}

