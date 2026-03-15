// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CheckKickHit.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API UCheckKickHit : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "VALUE")
	TArray < TObjectPtr<AActor>> HitActors;

	UPROPERTY(EditAnywhere, Category = "VALUE")
	FName HitPartName;

	UPROPERTY(EditAnywhere, Category = "VALUE")
	float SphereRadius = 50.0f;
};
