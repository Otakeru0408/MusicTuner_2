// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SetCollisionOn.generated.h"

/**
 *
 */
UCLASS()
class MUSICTUNER_2_API USetCollisionOn : public UAnimNotify
{
	GENERATED_BODY()
public:
	// ’Ê’m‚ªÀs‚³‚ê‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éƒƒCƒ“ŠÖ”
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
