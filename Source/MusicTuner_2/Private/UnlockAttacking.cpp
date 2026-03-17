// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockAttacking.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"

UUnlockAttacking::UUnlockAttacking() {

}

// ’Ê’m‚ªÀs‚³‚ê‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éƒƒCƒ“ŠÖ”
void UUnlockAttacking::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// ‚±‚±‚Éˆ—‚ğ‘‚­
	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (player) {
		player->isAttacking = false;
	}
}