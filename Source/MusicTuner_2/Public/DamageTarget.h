// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MUSICTUNER_2_API IDamageTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool DamageToEnemy(int32 DamageValue, AActor* DamageCauser, bool IsComboHit) { return true; }

	virtual bool CanDamage() { return true; }
};
