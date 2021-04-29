// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseAmmoPattern.h"
#include "UObject/Object.h"
#include "InfinityAmmo.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULE_API UInfinityAmmo : public UBaseAmmoPattern
{
	GENERATED_BODY()

public:
	virtual bool CanUseAmmo() override {return true;}
	virtual void UseAmmo() override {};
};
