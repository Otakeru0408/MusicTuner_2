// Fill out your copyright notice in the Description page of Project Settings.


#include "SetCollisionOn.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter.h"

// ’Ê’m‚ªÀs‚³‚ê‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éƒƒCƒ“ŠÖ”
void USetCollisionOn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)return;

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (player) {
		player->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

		MeshComp->SetCollisionProfileName(TEXT("Pawn"));
	}
}

