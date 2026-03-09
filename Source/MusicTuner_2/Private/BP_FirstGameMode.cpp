// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_FirstGameMode.h"
#include "BallPlayer.h"

ABP_FirstGameMode::ABP_FirstGameMode()
{
	DefaultPawnClass = ABallPlayer::StaticClass();
}