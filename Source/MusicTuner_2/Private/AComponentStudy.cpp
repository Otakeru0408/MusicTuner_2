// Fill out your copyright notice in the Description page of Project Settings.


#include "AComponentStudy.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAComponentStudy::AAComponentStudy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	//StaticMeshComponentを作成する
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Game/CPP_BP/Meshes/SM_SampleCube.SM_SampleCube"), NULL, LOAD_None, NULL);
	StaticMesh->SetStaticMesh(Mesh);

	//StaticMeshComponentをRootにアタッチする
	StaticMesh->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetRelativeLocation(FVector(50.0f, 0.f, 0.f));

	Arrow->SetupAttachment(StaticMesh);
	Arrow->SetArrowColor(FLinearColor(0.f, 0.f, 1.0f));

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->SetRelativeLocation(FVector(130.0f, 0.f, 0.f));

	PointLight->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void AAComponentStudy::BeginPlay()
{
	Super::BeginPlay();

	FString Message = TEXT("C++ Hello World");

	UKismetSystemLibrary::PrintString(
		this,
		Message,
		true,
		true,
		TextColor,
		Duration,
		TEXT("NONE")
	);
}
