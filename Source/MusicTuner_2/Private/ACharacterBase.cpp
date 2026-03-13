// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacterBase.h"
#include "SoundRingNext.h"

// Sets default values
//コンストラクタ
AACharacterBase::AACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AACharacterBase::BeginPlay()
{
	Super::BeginPlay();


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* world = GetWorld();
	if (SoundRingClass) UE_LOG(LogTemp, Display, TEXT("Instantiate"));
	if (world && SoundRingClass) {
		SoundRing = world->SpawnActor<ASoundRingNext>(
			SoundRingClass, GetActorLocation() + SoundRingOffset, GetActorRotation(), SpawnParams);

		if (SoundRing) {
			//SoundRing->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called every frame
void AACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AACharacterBase::GetEnemyForwardVector() const {
	return RootComponent->GetForwardVector();
}