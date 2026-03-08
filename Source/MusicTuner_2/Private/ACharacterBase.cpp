// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacterBase.h"

// Sets default values
AACharacterBase::AACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::PrintString(this, "C++ Hello World!", true, false, FColor::Cyan, 2.f);

	UE_LOG(LogTemp, Display, TEXT("Display Message"));

	FString message = TEXT("Duration : ");
	message += UKismetStringLibrary::Conv_DoubleToString(Duration);

	GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Cyan, message, true, FVector2D(2.0f, 2.0f));
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

