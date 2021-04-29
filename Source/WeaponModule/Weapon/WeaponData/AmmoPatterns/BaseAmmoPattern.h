// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponObject.h"
#include "BaseAmmoPattern.generated.h"

UCLASS(Abstract)
class WEAPONMODULE_API UBaseAmmoPattern : public UWeaponObject
{
	GENERATED_BODY()

	friend class UBaseFireMode;

public:
	virtual bool CanUseAmmo() {return true;}
	

protected:
	virtual void StopAll() {}
	

private:
	virtual void UseAmmo() {}
	
};
