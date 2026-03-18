// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardCrystal.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARewardCrystal::ARewardCrystal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = CrystalMesh;
	CrystalMesh->SetCollisionProfileName(TEXT("PickupProfile"));
	//CrystalMesh->SetGenerateOverlapEvents(false);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName(TEXT("PickupProfile"));
}

// Called when the game starts or when spawned
void ARewardCrystal::BeginPlay()
{
	Super::BeginPlay();

	if (SphereCollision)
	{
		// オーバーラップ開始時にOnOverlap関数を呼ぶように設定
		SphereCollision->OnComponentBeginOverlap.AddDynamic(
			this,
			&ARewardCrystal::OnMeshBeginOverlap
		);

		// SphereCollisionが見つかった場合の確認ログ
		UE_LOG(LogTemp, Warning, TEXT("SphereCollision found and saved to variable: %s"), *GetName());
	}
}

// Called every frame
void ARewardCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARewardCrystal::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (AMainCharacter* player = Cast<AMainCharacter>(OtherActor)) {
			player->OnCrystalGained();
			UGameplayStatics::PlaySound2D(GetWorld(), GainedSound);
			Destroy();
		}
	}
}